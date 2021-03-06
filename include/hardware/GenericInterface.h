#ifndef GENERICCONTROLLER_H
#define GENERICCONTROLLER_H

#include <ros/ros.h>
#include <diagnostic_updater/diagnostic_updater.h>
#include <diagnostic_updater/publisher.h>

#include <orbus_interface/GPIO.h>
#include <orbus_interface/Service.h>
#include <orbus_interface/BoardTime.h>
#include <orbus_interface/Peripheral.h>

#include "hardware/serial_controller.h"

namespace ORInterface
{

class GenericInterface : public diagnostic_updater::DiagnosticTask
{
public:
    GenericInterface(const ros::NodeHandle &nh, const ros::NodeHandle &private_nh, orbus::serial_controller *serial);

    void initializeDiagnostic();

    void run(diagnostic_updater::DiagnosticStatusWrapper &stat);

    void updateInterface();

protected:

    /**
     * @brief initialize Initialization all parts
     */
    void initialize();

    /**
     * @brief connectionCallback When a Publisher is connected launch this callback
     * @param pub information about the publisher
     */
    void connectionCallback(const ros::SingleSubscriberPublisher& pub);
    //Initialization object
    //NameSpace for bridge controller
    ros::NodeHandle mNh;
    ros::NodeHandle private_mNh;
    // Serial controller communication
    orbus::serial_controller *mSerial;
    // Diagnostic
    diagnostic_updater::Updater diagnostic_updater;
    //Name of device
    string code_date, code_version, code_author, code_board_type, code_board_name;
    // List of messages to send to the board
    vector<packet_information_t> information_frames;
private:
    /**
     * @brief systemFrame
     * @param option
     * @param type
     * @param command
     * @param message
     */
    void systemFrame(unsigned char option, unsigned char type, unsigned char command, message_abstract_u message);

    /**
     * @brief peripheralFrame
     * @param option
     * @param type
     * @param command
     * @param message
     */
    void peripheralFrame(unsigned char option, unsigned char type, unsigned char command, message_abstract_u message);

    void setupGPIO(std::vector<int> gpio_list);

    void convertGPIO(peripherals_gpio_port_t data);

    /**
     * @brief service_Callback Internal service to require information from the board connected
     * @param req
     * @param msg
     * @return
     */
    bool service_Callback(orbus_interface::Service::Request &req, orbus_interface::Service::Response &msg_system);

    bool gpio_Callback(orbus_interface::GPIO::Request &req, orbus_interface::GPIO::Response &msg_system);

    void gpio_subscriber_Callback(const orbus_interface::Peripheral::ConstPtr& msg);

    int binary_decimal(int n);

    // Service board
    ros::ServiceServer srv_board, srv_gpio;
    // time execution functions
    ros::Publisher pub_time;
    ros::Publisher pub_peripheral;
    // Subscriber peripherals
    ros::Subscriber sub_peripheral;
    // Message for pubblisher
    orbus_interface::BoardTime msg_system;
    orbus_interface::Peripheral msg_peripheral;

    peripheral_gpio_map_t gpio_map;
    std::vector<int> gpio_list;

};

}

#endif // GENERICCONTROLLER_H
