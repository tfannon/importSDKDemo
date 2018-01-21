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
            //self.showAlertViewWith(title: "Register App", message: msg)
        }
        else {
            //self.showAlertViewWith(title: "Register App", message: msg)
            //print ("startConnectionToProduct called")
            DJISDKManager.startConnectionToProduct()
            
        }
    }
    
    
    func productConnected(_ product: DJIBaseProduct?) {
        if let product = DJISDKManager.product() as? DJIAircraft {
            if let flightController = product.flightController {
                self.showAlertViewWith(title: "Register App", message: "flightController obtained")
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

    
    func showAlertViewWith(title: String, message: String) {
        let alert = UIAlertController(title: title, message: message, preferredStyle: .alert)
        let okAction = UIAlertAction(title: "OK", style: .default, handler: nil)
        alert.addAction(okAction)
        self.present(alert, animated: true, completion: nil)
    }
    
    func registerApp() {
        DJISDKManager.registerApp(with: self)
    }
    
    override func viewWillAppear(_ animated: Bool) {
        //self.registerApp()
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        self.locationManager.requestAlwaysAuthorization()

//        self.dbref = Database.database().reference()
//        dbref.child("leaderCoordinates").observe(DataEventType.value, with: { (snapshot) in
//            let value = snapshot.value as? String
//            print(value)
//            //self.debugInfo[0].value = value as? String ?? "error"
//            //self.tableView.reloadRows(at: [IndexPath(row: 0, section: 0)], with: UITableViewRowAnimation.none)
//        })
        
        if CLLocationManager.locationServicesEnabled() {
            locationManager.delegate = self
            locationManager.desiredAccuracy = kCLLocationAccuracyBest
            locationManager.distanceFilter = 0.1
            locationManager.startUpdatingLocation()
        }
        
        

        // Do any additional setup after loading the view, typically from a nib.
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

