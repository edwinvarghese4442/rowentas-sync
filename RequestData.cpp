#include <iostream>
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <strsafe.h>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include "SimConnect.h"

// Manual implementation of to_string function for compilers lacking support
template <typename T>
std::string to_string(const T& value) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

int quit = 0;
HANDLE hSimConnect = NULL;
const wchar_t* PORTNAME = L"COM3";// Change this to the appropriate port name
const int BAUD_RATE = 9600;


struct Struct1
{
    char    title[256];
    double  togdisp;
    double  togmach;
    double  togpb;
};

static enum DATA_DEFINE_ID {
    DEFINITION_1,
};

static enum DATA_REQUEST_ID {
    REQUEST_1,
};


// Function to send dictionary over serial port
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
            int val_togdisp = 0;
            int val_togmach = 0;
            int val_togpb = 0;
            DWORD ObjectID = pObjData->dwObjectID;
            Struct1* pS = (Struct1*)&pObjData->dwData;
            if (SUCCEEDED(StringCbLengthA(&pS->title[0], sizeof(pS->title), NULL))) // security check
            {
                //printf("\nObjectID=%d  Title=\"%s\"\nLat=%f  Lon=%f  brakepark=%f  Kohlsman=%.2f", ObjectID, pS->title, pS->latitude, pS->longitude, pS->altitude, pS->kohlsmann);
                //printf("\nObjectID=%d  Title=\"%s\ togdisp=%f togmach =%f togpb=%f", ObjectID, pS->title, pS->togdisp,  pS->togmach,  pS->togpb);
                //printf("Boolean value: %d\n", pS->bpi);
                val_togdisp = int(pS->togdisp);
                val_togmach = int(pS->togmach);
                val_togpb = int(pS->togpb);

                //std::string portName = "COM3"; // Serial port name for Windows
                std::string tempstr = "togdisp:" + std::to_string(val_togdisp) + ",togmach:" + std::to_string(val_togmach) + ",togpb:" + std::to_string(val_togpb) + ",togpxb:" + std::to_string(val_togpb);
                const char* valueStr = tempstr.c_str();


                // Send data via serial port
                if (sendDataViaSerial(PORTNAME, BAUD_RATE, valueStr)) {
                    std::cout << valueStr << std::endl;
                    std::cout << "Data sent successfully." << std::endl;
                }
                else {
                    std::cerr << "Failed to send data." << std::endl;
                }

                //std::map<std::string, int> dictionary = {
                //    {"togdisp", val_togdisp},
                //    {"togmach", val_togmach},
                //    {"togpbxx", val_togpb}
                //    // Add more key-value pairs as needed
                //};
                //for (const auto& pair : dictionary) {
                //    std::cout << pair.first << ": " << pair.second << std::endl;
                //}
                //sendDictionary(PORTNAME, dictionary);
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
        // Handle request failure
        printf("\nRequest data failed");

    }
}


void testDataRequest()
{
    HRESULT hr;

    if (SUCCEEDED(SimConnect_Open(&hSimConnect, "Request Data", NULL, 0, 0, 0)))
    {
        printf("\nConnected to Flight Simulator!");

        for (int i = 0; i < 1; i++)
        {
            // Set up the data definition, but do not yet do anything with it
            hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_1, "Title", NULL, SIMCONNECT_DATATYPE_STRING256);
            //hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_1, "Kohlsman setting hg", "inHg");
            hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_1, "L:A32NX_TRK_FPA_MODE_ACTIVE", "number", SIMCONNECT_DATATYPE_FLOAT64);
            hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_1, "AUTOPILOT MANAGED SPEED IN MACH", "number", SIMCONNECT_DATATYPE_FLOAT64);
            hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_1, "L:A32NX_PARK_BRAKE_LEVER_POS", "number", SIMCONNECT_DATATYPE_FLOAT64);

            //hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_1, "Plane Latitude", "degrees");
            //hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_1, "Plane Longitude", "degrees");
        }

        while (true)
        {
            RequestData();
            SimConnect_CallDispatch(hSimConnect, MyDispatchProcRD, NULL);
            Sleep(10); // Allow other threads to execute
        }

        // Cleanup
        SimConnect_Close(hSimConnect);
    }
}


int main() {

    testDataRequest();

    return 0;
}