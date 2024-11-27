#include "./ui_mainwindow.h"
#include "mainwindow.h"
#include "server.h"
#include "attacker.h"
#include "configureserverdialog.h"
#include "setupattackerdialog.h"
#include "simulationdialog.h"
#include <QtConcurrent/QtConcurrent>
#include <QMessageBox>
#include <vector>
#include <atomic>
#include <chrono>
#include <thread>

using namespace std;

Server* server = nullptr;
vector<unique_ptr<Attacker>> attackers;

bool attackRunning(true);
int batteryValue = 0;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_configureYourServerButton_clicked()
{
    hide();
    ConfigureServerDialog *configureServerDialog = new ConfigureServerDialog(this);
    configureServerDialog->show();

    connect(configureServerDialog->ui->goBackButton, &QPushButton::clicked, [this,configureServerDialog]()
    {
        configureServerDialog->hide();
        show();
    });

    connect(configureServerDialog->ui->confirmConfiguration, &QPushButton::clicked, this, [this, configureServerDialog]() {
        bool okQueueSize, okCapacity , okRateLimit;
        int queueSize = configureServerDialog->ui->serverQueueSizeInput->text().toInt(&okQueueSize);
        int rateLimit = configureServerDialog->ui->rateLimitInput->text().toInt(&okRateLimit);
        int capacity = configureServerDialog->ui->serverCapacityInput->text().toInt(&okCapacity);
        bool enableBlacklist = configureServerDialog->ui->serverBlacklistInput->isChecked();
        if (!okQueueSize || !okCapacity || queueSize < 1 || queueSize > 1000 || capacity <= 0 || capacity > 200 || rateLimit < 0 || rateLimit > 100) {
            QMessageBox::warning(this, "Invalid Input", "Please enter valid values.");
            return;
        }

        server = new Server(capacity, queueSize, rateLimit, enableBlacklist);
        configureServerDialog->hide();
        show();
    });
}

void MainWindow::on_setupYourAttackerButton_clicked()
{
    hide();

    setupAttackerDialog *SetupAttackerDialog = new setupAttackerDialog(this);
    SetupAttackerDialog->show();

    connect(SetupAttackerDialog->ui->goBackButton, &QPushButton::clicked, [this,SetupAttackerDialog]()
    {
        SetupAttackerDialog->hide();
        show();
    });

    connect(SetupAttackerDialog->ui->confirmSetup, &QPushButton::clicked, this, [this, SetupAttackerDialog]() {
        int attackerSize = SetupAttackerDialog->ui->attackerCountInput->text().toInt();
        int requestRate = SetupAttackerDialog->ui->attackerRPSInput->text().toInt();

        if(attackerSize <= 0 || attackerSize > 100 || requestRate <= 0 || requestRate > 10) {
            QMessageBox::warning(this, "Invalid Input", "Please enter valid values.");
            return;
        }


        for (int i = 0; i < attackerSize ; ++i) {
            auto attacker = make_unique<Attacker>("192.168.1." + std::to_string(i + 1), requestRate);
            attackers.push_back(std::move(attacker));
        }
        SetupAttackerDialog->hide();
        show();
    });
}

void MainWindow::on_runSimulationButton_clicked()
{
    if (server == nullptr) {
        QMessageBox::warning(this, "Error", "Please configure your server.");
        return;
    }
    if (attackers.size() == 0) {
        QMessageBox::warning(this, "Error", "Please setup your attackers first.");
        return;
    }

    hide();
    simulationDialog *SimulationDialog = new simulationDialog();
    SimulationDialog->show();
    SimulationDialog->ui->progressBar->setMinimum(0);
    SimulationDialog->ui->progressBar->setMaximum(server->queue_size_limit);

    simulateDdosAttack(SimulationDialog);
}

void MainWindow::simulateDdosAttack(simulationDialog* Dialog)
{
    attackRunning = true;
    QtConcurrent::run([this, Dialog]() {
        auto runningTime = chrono::high_resolution_clock::now();
        while (attackRunning) {

            for (int i = 0; i < attackers.size(); ++i) {
                for(int j = 0 ; j < attackers[i]->get_request_rate() ; j++)
                {
                    this_thread::sleep_for(std::chrono::milliseconds(100));
                Request req = attackers[i]->generate_request();
                if (server->add_request(req, Dialog)) {
                    batteryValue += (server->queue_size_limit / 100.0);
                    QMetaObject::invokeMethod(Dialog, [this,Dialog,&batteryValue]() {
                        emit Dialog->updateProgress(batteryValue);
                    });
                    runningTime = chrono::high_resolution_clock::now();
                }
                }
            }

                if (batteryValue > 0) {
                    int processed = server->process_requests(Dialog);
                    if (processed == -1) {
                        QMetaObject::invokeMethod(Dialog, [this,Dialog]() {
                            QMessageBox *box = new QMessageBox(this);
                            box->setStyleSheet("background-color: red;");
                            box->warning(this, "Error", "SERVER CRASHED DUE TO REQUEST OVERLOAD!");
                            emit Dialog->updateProgress(server->queue_size_limit);
                        });
                        attackRunning = false;
                        return;
                    }
                    batteryValue -= processed * (server->queue_size_limit / 100.0);
                    QMetaObject::invokeMethod(Dialog, [this,Dialog,&batteryValue]() {
                        emit Dialog->updateProgress(batteryValue);
                    });
                }
            QThread::msleep(10);
        }
    });
}


