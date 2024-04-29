#include <iostream>
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <strsafe.h>
#include <vector>
//#include "Arduino.h"
#include "SimConnect.h"

int quit = 0;
HANDLE hSimConnect = NULL;

//const int ledPin = 3; // Pin number for the LED
//
//void setup() {
//    pinMode(ledPin, OUTPUT); // Set the LED pin as an output
//}
struct Struct1
{
    char    title[256];
    double  kohlsmann;
    double  altitude;
    double  latitude;
    double  longitude;
};

static enum DATA_DEFINE_ID {
    DEFINITION_1,
};

static enum DATA_REQUEST_ID {
    REQUEST_1,
};

bool sendDataViaSerial(const wchar_t* portName, int baudRate, const char* data) {
    // Open serial port
    HANDLE hSerial = CreateFileW(portName, GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hSerial == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to open serial port." << std::endl;
        return false;
    }

    // Set serial port parameters
    DCB dcbSerialParams = { 0 };
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (!GetCommState(hSerial, &dcbSerialParams)) {
        std::cerr << "Failed to get serial port parameters." << std::endl;
        CloseHandle(hSerial);
        return false;
    }
    dcbSerialParams.BaudRate = baudRate;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;
    if (!SetCommState(hSerial, &dcbSerialParams)) {
        std::cerr << "Failed to set serial port parameters." << std::endl;
        CloseHandle(hSerial);
        return false;
    }

    // Write data to serial port
    DWORD bytesWritten;
    if (!WriteFile(hSerial, data, strlen(data), &bytesWritten, NULL)) {
        std::cerr << "Error writing to serial port." << std::endl;
        CloseHandle(hSerial);
        return false;
    }

    // Close serial port
    CloseHandle(hSerial);

    return true;
}



void CALLBACK MyDispatchProcRD(SIMCONNECT_RECV* pData, DWORD cbData, void* pContext)
{
    HRESULT hr;

    switch (pData->dwID)
    {
    case SIMCONNECT_RECV_ID_SIMOBJECT_DATA_BYTYPE:
    {
        SIMCONNECT_RECV_SIMOBJECT_DATA_BYTYPE* pObjData = (SIMCONNECT_RECV_SIMOBJECT_DATA_BYTYPE*)pData;

        switch (pObjData->dwRequestID)
        {
        case REQUEST_1:
        {
            int val_alt = 0;
            DWORD ObjectID = pObjData->dwObjectID;
            Struct1* pS = (Struct1*)&pObjData->dwData;
            if (SUCCEEDED(StringCbLengthA(&pS->title[0], sizeof(pS->title), NULL))) // security check
            {
                val_alt = int(pS->altitude);
                
                //printf("\nObjectID=%d  Title=\"%s\"\nLat=%f  Lon=%f  brakepark=%f  Kohlsman=%.2f", ObjectID, pS->title, pS->latitude, pS->longitude, pS->altitude, pS->kohlsmann);
                printf("\nObjectID=%d  Title=\"%s\"\nbrakepark=%f", ObjectID, pS->title, pS->altitude);

            }
            // Define serial port parameters
            const wchar_t* portName = L"COM3";// Change this to the appropriate port name
            const int baudRate = 9600; // Change this to the appropriate baud rate
            printf("Integer: %d\n", val_alt);
            // Define data to send
            const char* valueStr = "43";
            if (val_alt == 0) {
                valueStr = "43";
            }
            else {
                valueStr = "42";
            }

            // Send data via serial port
            if (sendDataViaSerial(portName, baudRate, valueStr)) {
                std::cout << valueStr << std::endl;
                std::cout << "Data sent successfully." << std::endl;
            }
            else {
                std::cerr << "Failed to send data." << std::endl;
            }
        }

        default:
            break;
        }
        break;
    }


    case SIMCONNECT_RECV_ID_QUIT:
    {
        quit = 1;
        break;
    }

    default:
        printf("\nReceived:%d", pData->dwID);

    }
}

void RequestData()
{
    // Request data from SimConnect
    HRESULT hr = SimConnect_RequestDataOnSimObjectType(hSimConnect, REQUEST_1, DEFINITION_1, 0, SIMCONNECT_SIMOBJECT_TYPE_USER);
    if (FAILED(hr))
    {
        printf("\nRequest data failed");
        // Handle request failure
    }
}


void testDataRequest()
{
    HRESULT hr;

    if (SUCCEEDED(SimConnect_Open(&hSimConnect, "Request Data", NULL, 0, 0, 0)))
    {
        printf("\nConnected to Flight Simulator!");

        // Set up the data definition, but do not yet do anything with it
        hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_1, "Title", NULL, SIMCONNECT_DATATYPE_STRING256);
        hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_1, "Kohlsman setting hg", "inHg");
        hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_1, "BRAKE PARKING INDICATOR", "Bool");
        hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_1, "Plane Latitude", "degrees");
        hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_1, "Plane Longitude", "degrees");

        // Main loop
        while (true)
        {
            RequestData();
            SimConnect_CallDispatch(hSimConnect, MyDispatchProcRD, NULL);
            Sleep(20); // Allow other threads to execute
        }

        // Cleanup
        SimConnect_Close(hSimConnect);
    }
}

/*int _tmain(int argc, _TCHAR* argv[])
{
    testDataRequest();

    return 0;
}*/
int main() {
    
    testDataRequest();

    return 0;
}