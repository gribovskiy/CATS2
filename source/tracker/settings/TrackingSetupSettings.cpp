#include "TrackingSetupSettings.hpp"

#include "TrackingSettings.hpp"

#include <settings/CommandLineParameters.hpp>
#include <settings/CalibrationSettings.hpp>
#include <settings/GrabberSettings.hpp>

#include <QtCore/QDebug>

/*!
 * Initializes the parameters for the tracking setup of given type.
 */
bool TrackingSetupSettings::init(QString configurationFilePath,
                                 SetupType::Enum setupType,
                                 bool needCalibration)
{
    // first check that the calibration settings are initialized
    if (CalibrationSettings::get().init(configurationFilePath, setupType) || (!needCalibration)) {
        // then check that the input stream is set
        if (CommandLineParameters::get().cameraDescriptor(setupType).isValid()) {
            // now check that the grabber settings are initialized
            if (GrabberSettings::get().init(configurationFilePath, setupType)) {
                // finally check that the tracking routine is set for given setup type
                if (TrackingSettings::get().init(configurationFilePath, setupType)) {
                    return true;
                } else {
                    qDebug() << "Couldn't setup the tracking routine, finished";
                    return false;
                }
            } else {
                qDebug() << "Couldn't setup grabber settings, finished";
                return false;
            }
        } else {
            qDebug() << "Main camera descriptor is ill-defined";
            return false;
        }
    } else {
        qDebug() << "Couldn't setup calibration settings, finished";
        return false;
    }
}



