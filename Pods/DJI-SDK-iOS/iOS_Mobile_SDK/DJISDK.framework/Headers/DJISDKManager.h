//
//  DJISDKManager.h
//  DJISDK
//
//  Copyright © 2015, DJI. All rights reserved.
//

#import <DJISDK/DJISDKFoundation.h>


/**
 *  To register the app, create a new key in the info.plist file where the plist key
 *  is "DJISDKAppKey" and its value is your DJI registered App key.
 */
#define SDK_APP_KEY_INFO_PLIST_KEY @"DJISDKAppKey"

NS_ASSUME_NONNULL_BEGIN
@class DJIAircraft;
@class DJIBaseProduct;
@class DJIBaseComponent;
@class DJIBluetoothProductConnector;
@class DJIKeyManager;
@class DJIFlyZoneManager;
@class DJIMissionControl;
@class DJIVideoFeeder;
@class DJIUserAccountManager;
@class DJIAppActivationManager; 
@class DJILDMManager;


/**
 *  This protocol provides delegate methods to receive the updated registration
 *  status and the change of the  connected product.
 */
@protocol DJISDKManagerDelegate <NSObject>

@required


/**
 *  Delegate method after the application attempts to register.
 *  
 *  @param error `nil` if registration is successful. Otherwise it contains an `NSError` object with error codes from `DJISDKRegistrationError`.
 */
- (void)appRegisteredWithError:(NSError *_Nullable)error;

@optional


/**
 *  Called when the "product" is connected.
 *  
 *  @param product Product object. nil if the USB link or WiFi link between the product and phone is  disconnected.
 */
- (void)productConnected:(DJIBaseProduct *_Nullable)product;


/**
 *  Called when the "product" is disconnected.
 */
- (void)productDisconnected;


/**
 *  Called when the "component" is connected.
 *  
 *  @param key Key of the component.
 *  @param index Index of the component.
 */
- (void)componentConnectedWithKey:(NSString * _Nullable)key andIndex:(NSInteger)index;


/**
 *  Called when the "component" is disconnected.
 *  
 *  @param key Key of the component.
 *  @param index Index of the component.
 */
- (void)componentDisconnectedWithKey:(NSString * _Nullable)key andIndex:(NSInteger)index;

@end


/**
 *  This class is the entry point for using the SDK with a DJI product.  Most
 *  importantly, this class is used to register the SDK, and to connect  to and
 *  access the product. This class also provides access to important feature
 *  managers (such as `keyManager`), debugging tools, and threading control of
 *  asynchronous completion blocks. SDK Registration using `registerAppWithDelegate`
 *  must be successful before the SDK can be used with a DJI product.
 */
@interface DJISDKManager : NSObject

- (instancetype)init OBJC_UNAVAILABLE("You must use the singleton");

+ (instancetype)new OBJC_UNAVAILABLE("You must use the singleton");


/**
 *  Used to manage the DJI account of users. Login is required by
 *  `DJIFlyZoneManager`  and `DJIAppActivationManager`.
 *  
 *  @return A instance of `DJIUserAccountManager`.
 */
+ (DJIUserAccountManager *)userAccountManager;


/**
 *  Used to check the states related to the App Activation.
 *  
 *  @return A instance of `DJIAppActivationManager`.
 */
+ (DJIAppActivationManager *)appActivationManager;


/**
 *  The DJI product which is connected to the mobile device, only available after
 *  successful registration of the app.
 *  
 *  @return A instance of `DJIBaseProduct`.
 */
+ (__kindof DJIBaseProduct *_Nullable)product;



/**
 *  Used to establish the Bluetooth connection between the mobile device and the DJI
 *  product. The Bluetooth connection needs to be established before a connection
 *  between the SDK and the DJI product can be made using
 *  `startConnectionToProduct`.
 *  
 *  @return A `DJIBluetoothProductConnector` instance.
 */
+ (nullable DJIBluetoothProductConnector *)bluetoothProductConnector;


/**
 *  The first time the app is initialized after installation, the app connects to a
 *  DJI Server through the  internet to verify the Application Key. Subsequent app
 *  starts will use locally cached verification  information to register the app.
 *  
 *  @param delegate Delegate used for both the registration result, and when the product changes.
 */
+ (void)registerAppWithDelegate:(id<DJISDKManagerDelegate>)delegate;


/**
 *  Queue in which completion blocks are called. If left unset,  completion blocks
 *  are called in main queue.
 *  
 *  @param completionBlockQueue Dispatch queue.
 */
+ (void)setCompletionBlockQueue:(dispatch_queue_t)completionBlockQueue;


/**
 *  Starts a connection between the SDK and the DJI product. This method should be
 *  called  after successful registration of the app and once there is a data
 *  connection between the mobile device and DJI product. This data connection is
 *  either a USB cable connection, a WiFi connection (that needs to be established
 *  outside of the SDK) or a Bluetooth connection (that needs to be established with
 *  `bluetoothProductConnector`). If the connection succeeds, `productConnected`
 *  will be called if the connection succeeded. Returns `YES` if the connection is
 *  started successfully. For products which connect to the mobile device using
 *  Bluetooth, `bluetoothProductConnector` should be used to get a
 *  `DJIBluetoothProductConnector` object which can handle Bluetooth device
 *  connection.
 *  
 *  @return `YES` if the connection is started successfully.
 */
+ (BOOL)startConnectionToProduct;


/**
 *  Disconnect from the connected DJI product.
 */
+ (void)stopConnectionToProduct;


/**
 *  Set the SDK to close the connection automatically when the app enters the
 *  background, and resume connection  automatically when the app enters the
 *  foreground. Default is `YES`.
 *  
 *  @param isClose `YES` if the connection should be closed when entering background.
 */
+ (void)closeConnectionWhenEnteringBackground:(BOOL)isClose;


/**
 *  Gets the DJI Mobile SDK Version. Returns SDK version as a string.
 *  
 *  @return An NSString object.
 */
+ (NSString *)SDKVersion;


/**
 *  Registration state.
 *  
 *  @return `YES` if SDK is registered.
 */
+ (BOOL)hasSDKRegistered;


/**
 *  Enter debug mode with debug IP. Please download and use the latest DJI SDK
 *  Bridge app from App Store: https://itunes.apple.com/us/app/sdk-
 *  bridge/id1263583917?ls=1&mt=8
 *  
 *  @param bridgeAppIP Debug IP of the DJI Bridge App.
 */
+ (void)enableBridgeModeWithBridgeAppIP:(NSString *)bridgeAppIP;


/**
 *  Enable remote logging with log server URL.
 *  
 *  @param deviceID Optional device ID to uniquely identify logs from an installation.
 *  @param url URL of the remote log server.
 */
+ (void)enableRemoteLoggingWithDeviceID:(NSString *_Nullable)deviceID logServerURLString:(NSString *)url;


/**
 *  Gets the path that flight logs are stored to. Flight logs are automatically
 *  generated and  stored on the mobile device. The SDK does nothing with these
 *  logs, and they are provided  only as a convenience for developers and users.
 *  Users can use these flight logs with DJI  service centers if they are making a
 *  warranty claim. Only developers using the SDK, and  users of the mobile device
 *  can access these logs. Older flight logs are overwritten by  newer flight logs
 *  over time, so the flight log path is given in case an application needs  to
 *  store all logs.
 *  
 *  @return An NSString object of the flight log path.
 */
+ (NSString *)getLogPath;

/*********************************************************************************/
#pragma mark - Keyed Interface
/*********************************************************************************/


/**
 *  Provide access to the SDK Key interface.
 *  
 *  @return An instance of `DJIKeyManager`.
 */
+ (nullable DJIKeyManager *)keyManager;

/*********************************************************************************/
#pragma mark - Fly Zone Manager
/*********************************************************************************/


/**
 *  Provide access to `DJIFlyZoneManager` used to manage DJI's GEO system for no fly
 *  zones.
 *  
 *  @return An instance of `DJIFlyZoneManager`.
 */
+ (nullable DJIFlyZoneManager *)flyZoneManager;

/*********************************************************************************/
#pragma mark - Mission Control
/*********************************************************************************/


/**
 *  Provide access to `DJIMissionControl` used to manage missions.
 *  
 *  @return An instance of `DJIMissionControl`.
 */
+ (nullable DJIMissionControl *)missionControl;

/*********************************************************************************/
#pragma mark - Video Feeder
/*********************************************************************************/


/**
 *  Provide access to `DJIVideoFeeder` used to video feeder.
 *  
 *  @return An instance of `DJIVideoFeeder`.
 */
+ (nullable DJIVideoFeeder *)videoFeeder;


/*********************************************************************************/
#pragma mark - Debug Log System
/*********************************************************************************/


/**
 *  Enables the debug log system. It will enable the DJI Mobile SDK to collect logs
 *  that are  related to the sdk's internal logic. These logs can be used to help
 *  diagnose SDK bugs.  The storage limit is 100 MB. When the limit is met, SDK will
 *  remove the older half of  the logs. By default, the debug log system is
 *  disabled. The developer should call this  method in each life cycle of the
 *  application to enable the debug log system. The logs  will be saved to directory
 *  named DJISDKDebugLogs. The logs can be accessed through iTunes.
 */
+ (void)enableDebugLogSystem;


/**
 *  Disables the debug log system. Calling this method will not remove the existing
 *  logs. Use `cleanDebugLogs`.
 */
+ (void)disableDebugLogSystem;


/**
 *  Cleans the existing logs. This method can be called when the debug log system is
 *  disabled.
 *  
 *  @return `YES` if all the logs are removed.
 */
+ (BOOL)cleanDebugLogs;

/*********************************************************************************/
#pragma mark - Local Data Mode (LDM)
/*********************************************************************************/


/**
 *  Manages Local Data Mode (LDM) functionality. Local data mode gives the developer
 *  the option to put the SDK into airplane mode, restricting its access to the
 *  internet.  See `DJILDMManager` for details on when and where this is possible,
 *  and what is restricted.
 *  
 *  @return An instance of `DJILDMManager`.
 */
+ (DJILDMManager *)ldmManager;

@end

NS_ASSUME_NONNULL_END
