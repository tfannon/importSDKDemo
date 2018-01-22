//
//  ViewController.swift
//  importSDKDemo
//
//  Created by Tommy Fannon on 1/20/18.
//  Copyright © 2018 StingrayDev. All rights reserved.
//

import UIKit
import DJISDK
import FirebaseDatabase
import CoreLocation

class ViewController: UIViewController, DJISDKManagerDelegate, DJIFlightControllerDelegate, CLLocationManagerDelegate {
    
    var dbref : DatabaseReference!
    let locationManager = CLLocationManager()
    var userLocation: CLLocationCoordinate2D??
    var flightController: DJIFlightController!
    
    @IBAction func coordinatesClicked(_ sender: Any) {
        print ("doItClicked")
        
        if let userLocation = locationManager.location?.coordinate {
            lblCoordinates.text = "Latitude: \(userLocation.latitude)  Longitude: \(userLocation.longitude)"
        }
    }
    
    @IBAction func doItClicked(_ sender: Any) {
    }
    
    @IBOutlet weak var lblCoordinates: UILabel!
    @IBOutlet weak var lblDoIt: UILabel!
    
   
    func appRegisteredWithError(_ error: Error?) {
        var msg = "Register succeeded"
        if (error != nil) {
            msg = "Register failed"
            self.showAlertViewWith(title: "Register App", message: msg)
        }
        else {
            //self.showAlertViewWith(title: "Register App", message: msg)
            //print ("startConnectionToProduct called")
            DJISDKManager.startConnectionToProduct()
        }
    }
    
    
    func productConnected(_ product: DJIBaseProduct?) {
        if self.flightController != nil {
            return
        }
        if let product = DJISDKManager.product() as? DJIAircraft,
            product.flightController != nil {
            self.flightController = product.flightController
            self.showAlertViewWith(title: "Register App", message: "flightController obtained") {
                self.dbref.child("tommyScratch").observe(DataEventType.value, with: { (snapshot) in
                    if let value = snapshot.value as? String {
                        self.showAlertViewWith(title: "Beacon change", message: value)
                    }
                })
            }
        }
        else {
            self.showAlertViewWith(title: "Register App", message: "failed to obtain flightController ")
        }
    }
    
    class func fetchFlightController() -> DJIFlightController? {
        if let product = DJISDKManager.product() as? DJIAircraft {
            return product.flightController
        }
        return nil
    }

    
    func showAlertViewWith(title: String, message: String, completion: (() -> ())? = nil) {
        let alert = UIAlertController(title: title, message: message, preferredStyle: .alert)
        let okAction = UIAlertAction(title: "OK", style: .default, handler: nil)
        alert.addAction(okAction)
        self.present(alert, animated: true, completion: completion)
    }
    
    func registerApp() {
        DJISDKManager.registerApp(with: self)
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        self.locationManager.requestAlwaysAuthorization()
        self.dbref = Database.database().reference()
        if CLLocationManager.locationServicesEnabled() {
            locationManager.delegate = self
            locationManager.desiredAccuracy = kCLLocationAccuracyBest
            locationManager.distanceFilter = 0.1
            locationManager.startUpdatingLocation()
        }
    }
    
    override func viewDidAppear(_ animated: Bool) {
        super.viewDidAppear(animated)
        self.registerApp()
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }


}

