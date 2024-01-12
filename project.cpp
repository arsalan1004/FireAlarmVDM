#include <iostream>
#include <string>
#include <thread>
#include <chrono>
using namespace std;

//-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x DEFINING TYPES x-x-x-x-x-x-x-x-x-x-x-xx-x-x-x-x-x

// alarm signal
enum alarmSignal
{
    ACTIVATE,
    DEACTIVATE
} asignal;

// sensor signal
enum sensorSignal
{
    CHANGE_DETECTED,
    NO_CHANGE
} signal;

//-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x STATE INVARIANT -x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x

class Invariant
{
public:
    virtual bool invCheck() = 0;
};

//-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x VDM CLASS -x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x

class VDM
{
public:
    bool invTest(bool expression)
    {
        if (!expression)
        {
            throw runtime_error("VDMException: State Invariant not satisfied");
        }
        else
        {
            cout << "System successfully initialized" << endl;
            return true;
        }
    }

    bool preTest(bool expression)
    {
        if (!expression)
        {
            throw runtime_error("VDMException: Pre-condition not satisfied");
        }
        return true;
    }
};

//-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x FIRE ALARM CLASS -x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x

class FireAlarmSystem : public Invariant, public VDM
{
public:
    double thresholdTemp;
    double environmentTemp;
    bool alarmIsRinging;
    bool heatDetected;
    double soundIntensity;

public:
    //-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x DEFINING THE VALUES -x-x-x-x-x-x-x-x-x-x-x-x-x-x-x
    const double MIN = 0;

    //-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x STATE INITIALIZER -x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x
    FireAlarmSystem(int t, int e, int a, int h, int s)
    {

        thresholdTemp = t;
        environmentTemp = e;
        alarmIsRinging = a;
        heatDetected = h;
        soundIntensity = s;
        invCheck();
    }

    //-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x INVARIANT CHECK FUNCTION -x-x-x-x-x-x-x-x-x-x-x-x-x
    bool invCheck()
    {
        bool expression = ((this->thresholdTemp >= MIN || this->thresholdTemp == -1) &&
                           (this->environmentTemp >= MIN || this->environmentTemp == -1) &&
                           (this->soundIntensity > MIN || this->soundIntensity == 0) &&
                           (this->heatDetected == false) && (this->alarmIsRinging == false));
        if (invTest(expression))
        {
            return true;
        }
        return false;
    }

    //-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x CLASS OPERATIONS x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x

    void setThresholdTemp(double inputTemp)
    {

        bool check = preTest(inputTemp > MIN);

        if (check)
        {
            this->thresholdTemp = inputTemp;
            cout << endl
                 << "Threshold temperature successfully updated!" << endl;
        }
    }

    void getEnvironmentTemp(double inputTemp)
    {
        bool check = preTest(inputTemp > MIN && thresholdTemp != -1);

        if (check)
        {
            environmentTemp = inputTemp;
            cout << endl
                 << "Environment temperature successfully updated!" << endl;
        }
    }

    bool detectChange()
    {
        bool check = preTest(thresholdTemp != -1 && environmentTemp != -1);

        if (check)
        {
            if (environmentTemp >= thresholdTemp)
            {
                heatDetected = true;
                return true;
            }
            else
            {
                heatDetected = false;
                return false;
            }
        }
    }

    void triggerAlarm()
    {
        bool check = preTest(heatDetected == true && (environmentTemp >= thresholdTemp));

        if (check)
        {
            asignal = ACTIVATE;
            alarmIsRinging = true;
            cout << "//////////// ALARM IS RINGING at Sound Intensity " << soundIntensity << " ////////////" << endl;
        }
    }

    void deactivateAlarm()
    {
        bool check = preTest(alarmIsRinging == true && (environmentTemp < thresholdTemp));

        if (check)
        {
            asignal = DEACTIVATE;
            alarmIsRinging = false;
            environmentTemp = thresholdTemp - 10;
            cout << "//////////// ALARM DEACTIVATED ////////////" << endl;
        }
    }

    void adjustVolume(double sound)
    {
        bool check = preTest(sound > 0);

        if (check)
        {
            soundIntensity = sound;
            cout << endl
                 << "Alarm Sound Intensity successfully updated!" << endl
                 << endl;
        }
    }

    void getSystemStats()
    {
        cout << endl
             << "System Threshold Temperature: " << thresholdTemp << endl;
        cout << "Current Environment Temperature: " << environmentTemp << endl;
        cout << "System sound intensity: " << soundIntensity << endl;
        cout << "Alarm ringing status: " << alarmIsRinging << endl;
        cout << "Heat Detection Status : " << heatDetected << endl
             << endl;
    }
};

//-x-x-x-x-x-x-x-x-x-x-x-x-x-x-x FIREALARMSYSTEM TESTER CLASS -x-x-x-x-x-x-x-x-x-x-x-x-x-x

class FireAlarmSystemTester
{
public:
    void startSystem()
    {
        try
        {
            cout << '\t' << '\t' << "FIRE ALARM SYSTEM" << endl
                 << endl;
            FireAlarmSystem system(-1, -1, false, false, 0);
            int choice;
            double temp;
            double sound;

            do
            {
                try
                {
                    cout << endl
                         << endl
                         << "DISPLAYING MENU" << endl
                         << endl;

                    cout << "1: Show System Variables " << endl
                         << "2: Specify Threshold Temperature" << endl
                         << "3: Get Environment Temperature " << endl
                         << "4: Adjust Sound Intensity " << endl
                         << "5: EXIT" << endl
                         << endl;

                    cout << "Enter Choice: ";
                    cin >> choice;

                    switch (choice)
                    {

                    case 1:
                        cout << endl
                             << "SHOWING SYSTEM CONDITIONS" << endl;
                        option1(system);
                        break;
                    case 2:
                        cout << endl
                             << "Enter desired Threshold Temperature: ";
                        cin >> temp;
                        option2(system, temp);
                        break;
                    case 3:
                        cout << endl
                             << "Enter current Enivronment Temperature: ";
                        cin >> temp;
                        option3(system, temp);
                        break;
                    case 4:

                        cout << endl
                             << "Enter Required Sound Intensity: ";
                        cin >> sound;
                        option4(system, sound);
                        break;

                    default:
                        break;
                    }
                }
                catch (const std::exception &e)
                {
                    std::cerr << '\n'
                              << e.what() << '\n'
                              << '\n';
                }

            } while (choice != 5);
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
            cout << "Initialization failed " << endl
                 << "Enter valid input fields" << endl
                 << endl;
        }
    }

    void option1(FireAlarmSystem &system)
    {
        system.getSystemStats();
    }

    void option2(FireAlarmSystem &system, double inputTemp)
    {
        system.setThresholdTemp(inputTemp);
    }
    void option3(FireAlarmSystem &system, double inputTemp)
    {
        system.getEnvironmentTemp(inputTemp);
        bool check = system.detectChange();
        double temp;
        if (check)
        {
            signal = CHANGE_DETECTED;
            cout << "SENSOR SIGNAL: CHANGE_DETECTED" << endl;
            char deactivate;

            while (deactivate != 'd')
            {
                asignal = ACTIVATE;
                cout << endl
                     << "ALARM SIGNAL: ACTIVATE" << endl;
                system.triggerAlarm();

                cout << "Enter d to deactivate: ";
                cin >> deactivate;

                if (deactivate == 'd')
                {
                    do
                    {
                        cout << endl
                             << "Decrease Environment Temperature to deactivate: ";
                        cin >> temp;
                        if (temp < system.thresholdTemp)
                        {
                            system.environmentTemp = temp;
                            asignal = DEACTIVATE;
                            cout << endl
                                 << "ALARM SIGNAL: DEACTIVATE" << endl;
                            system.deactivateAlarm();
                            system.detectChange();
                        }
                        else
                        {
                            cout << "Temperature still too high, decrease it further" << endl
                                 << endl;
                        }
                    } while (temp >= system.thresholdTemp);
                }
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
        }
        else
        {
            signal = NO_CHANGE;
            cout << "SENSOR SIGNAL: NO_CHANGE" << endl
                 << "Alarm in DEACTIVATE Status" << endl
                 << endl;
        }
    }
    void option4(FireAlarmSystem &system, double sound)
    {
        system.adjustVolume(sound);
    }
};

int main()
{
    FireAlarmSystemTester fireSystem;

    fireSystem.startSystem();

    return 0;
}