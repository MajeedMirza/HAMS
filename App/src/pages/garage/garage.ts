import { Component } from '@angular/core';
import { NavController, NavParams } from 'ionic-angular';
import { NodeServiceProvider } from '../../providers/node-service/node-service';

@Component({
  selector: 'page-garage',
  templateUrl: 'garage.html',
})
export class GaragePage {

  isOperating = false;
  garageTime = 16000; //Time it takes to open/close garage

  constructor(public navCtrl: NavController, 
    public navParams: NavParams, 
    public nodeServiceProvider: NodeServiceProvider) {
  }

  ionViewDidLoad() {
    console.log('ionViewDidLoad GaragePage');
  }

  open() {
    this.isOperating = true;
    this.nodeServiceProvider.openGarage();
    this.delay(this.garageTime).then(() => {
      this.isOperating = false;
    });
  }

  close() {
    this.nodeServiceProvider.closeGarage();
  }

  delay(ms: number) {
    return new Promise(resolve => setTimeout(resolve, ms));
  }

}
