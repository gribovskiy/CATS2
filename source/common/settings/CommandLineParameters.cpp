#include "CommandLineParameters.hpp"

#include <QtCore/QDebug>

/*!
 * Constructor.
 */
CommandLineParameters::CommandLineParameters()
{
}

/*!
 * Destructor.
 */
CommandLineParameters::~CommandLineParameters()
{
    qDebug() << Q_FUNC_INFO << "Destroying the object";
}

/*!
 * The singleton getter.
 */
CommandLineParameters& CommandLineParameters::get()
{
    static CommandLineParameters instance;
    return instance;
}

/*!
 * Initializes the parameters from the command line arguments.
 * This part is inspired by this stackoverflow post
 * http://stackoverflow.com/questions/865668/how-to-parse-command-line-arguments-in-c
 */
bool CommandLineParameters::init(int argc, char** argv, bool needConfigFile, bool needMainCamera, bool needBelowCamera)
{
    bool settingsAccepted = false;

    // first check for the "-h"/"--help" request
    if ((std::find(argv, argv + argc, "-h") != (argv + argc))
        || (std::find(argv, argv + argc, "--help") != (argv + argc))) {
        printSupportedArguments();
        // if the user asks for a help then he/she doesn't know what is happening,
        // hence need to ignore the rest of arguments
        return settingsAccepted;
    }

    // read the main camera parameters
    QString streamType;
    QString streamParameters;
    bool foundMainCameraParameters =
            (CommandLineParser::parseTypedArgument(argc, argv,
                                                   "-mc", streamType, streamParameters) ||
             CommandLineParser::parseTypedArgument(argc, argv,
                                                   "--maincam", streamType, streamParameters));
    if (foundMainCameraParameters) {
        // the main camera
        m_cameraDescriptors[SetupType::MAIN_CAMERA] = StreamDescriptor(streamType, streamParameters);
    } else {
        qDebug() << Q_FUNC_INFO << "Couldn't find the valid main camera parameters";
    }
    settingsAccepted = (foundMainCameraParameters || (!needMainCamera));

    // read the below camera parameters
    bool foundBelowCameraParameters =
            (CommandLineParser::parseTypedArgument(argc, argv, "-bc", streamType, streamParameters) ||
             CommandLineParser::parseTypedArgument(argc, argv, "--belowcam", streamType, streamParameters));
    if (foundBelowCameraParameters) {
        // the below camera
        m_cameraDescriptors[SetupType::CAMERA_BELOW] = StreamDescriptor(streamType, streamParameters);
    } else {
        qDebug() << Q_FUNC_INFO << "Couldn't find the valid camera below parameters";
    }
    settingsAccepted = settingsAccepted && (foundBelowCameraParameters || (!needBelowCamera));

    // get the configuration file path
    QString filePath;
    bool foundConfigFilePath =
            (CommandLineParser::parseArgument(argc, argv, "-c", filePath) ||
             CommandLineParser::parseArgument(argc, argv, "--config", filePath));
    if (foundConfigFilePath) {
        m_configurationFilePath = filePath;
    } else {
        qDebug() << Q_FUNC_INFO << "Couldn't find the configuration file";
    }
    settingsAccepted = settingsAccepted && (foundConfigFilePath || (!needConfigFile));

    return settingsAccepted;
}

/*!
 * Prints out the command line arguments expected by the video grabber.
 */
void CommandLineParameters::printSupportedArguments()
{
    qDebug() << "Video grabber usage";
    qDebug() << "\t -h --help\t\tShow this help message";
    qDebug() << "\t -mc --maincam\tDefines the video stream used to track agents."
             << "Format : -mc <StreamType> <parameters>";
    qDebug() << "\t -bc --belowcam\tDefines the video stream used to track the"
             << "robot under the aquarium. Format : -mc <StreamType> <parameters>";
    qDebug() << "\t -c --config\tThe configuration file. Format : -c <PathToFile>";
}



/*!
 * Looks for the given stream's argument in the command line.
 */
bool CommandLineParameters::parseStreamArguments(int argc, char** argv,
                                                 QString argument,
                                                 QString& streamType,
                                                 QString& parameters)
{
    QString argumentType;
    QString values;
    bool status = CommandLineParser::parseTypedArgument(argc, argv, argument, argumentType, values);
    if (status) {
        if (StreamDescriptor::isValidStreamType(argumentType)){
            streamType = argumentType;
            parameters = values;
            return true;
        } else {
            qDebug() << Q_FUNC_INFO << "Invalid argument type" << argumentType;
        }
    }
    return false;
}
