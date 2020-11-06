#include "example_helper.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ajile/HostSystem.h>
#include <ajile/ControllerDriver.h>

int RunExample(aj::Project (*createFunction)(unsigned short, unsigned int, float, std::vector<aj::Component>), int argc, char *argv[]) {

    // default connection settings
    char ipAddress[32] = "192.168.200.1";
    char netmask[32] = "255.255.255.0";
    char gateway[32] = "0.0.0.0";
    unsigned short port = 5005;
    CommunicationInterfaceType_e commInterface = aj::USB2_INTERFACE_TYPE;
    int deviceNumber = 0;

    // default sequence settings
    unsigned int repeatCount = 0; // repeat forever
    float frameTime_ms = -1.0; // frame time in milliseconds
    unsigned short sequenceID = 1;

    // read command line arguments
    for (int i=1; i<argc; i++) {
        if (strcmp(argv[i], "-i") == 0) {
            strcpy(ipAddress, argv[++i]);
        } else if (strcmp(argv[i], "-r") == 0) {
            repeatCount = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-f") == 0) {
            frameTime_ms = atof(argv[++i]);
        } else if (strcmp(argv[i], "--usb3") == 0) {
            commInterface = aj::USB3_INTERFACE_TYPE;
        } else if (strcmp(argv[i], "-d") == 0) {
            deviceNumber = atoi(argv[++i]);            
        } else {
            printf("Usage: %s [-i <IP address>] [-r <repeat count>] [-f <frame rate in ms>] [--usb3] [-d <deviceNumber>]\n", argv[0]);
            exit(2);
        }
    }

    // connect to the device
    aj::HostSystem ajileSystem;
    ajileSystem.SetConnectionSettingsStr(ipAddress, netmask, gateway, port);
    ajileSystem.SetCommunicationInterface(commInterface);
    ajileSystem.SetUSB3DeviceNumber(deviceNumber);
    if (ajileSystem.StartSystem() != aj::ERROR_NONE) {
        printf("Error starting AjileSystem.\n");
        exit(-1);
    }

    // create the project
    aj::Project project = createFunction(sequenceID, repeatCount, frameTime_ms, ajileSystem.GetProject()->Components());

    // get the first valid component index which will run the sequence
    bool wasFound = false;
    const aj::Sequence& sequence = project.FindSequence(sequenceID, wasFound);
    if (!wasFound) exit(-1);
    int componentIndex = ajileSystem.GetProject()->GetComponentIndexWithDeviceType(sequence.HardwareType());

    // stop any existing project from running on the device
    ajileSystem.GetDriver()->StopSequence(componentIndex);

    // load the project to the device
    ajileSystem.GetDriver()->LoadProject(project);
    ajileSystem.GetDriver()->WaitForLoadComplete(-1);

    for (map<int, aj::Sequence>::const_iterator iter = project.Sequences().begin(); iter != project.Sequences().end(); iter++) {
        const Sequence& sequence = iter->second;
        // run the project
        if (frameTime_ms >= 0)
            printf("Starting sequence %d with frame rate %f and repeat count %d\n", sequence.ID(), frameTime_ms, repeatCount);

        ajileSystem.GetDriver()->StartSequence(sequence.ID(), componentIndex);
    
        // wait for the sequence to start
        printf("Waiting for sequence %d to start\n", sequence.ID());
        while (ajileSystem.GetDeviceState(componentIndex)->RunState() != aj::RUN_STATE_RUNNING) ;

        if (repeatCount == 0) {
            printf("Sequence repeating forever. Press Enter to stop the sequence\n");
            getchar();
            ajileSystem.GetDriver()->StopSequence(componentIndex);
        }

        printf("Waiting for the sequence to stop.\n");
        while (ajileSystem.GetDeviceState(componentIndex)->RunState() == aj::RUN_STATE_RUNNING) ;
    }
}
