import { Component } from '@angular/core';
import { NavController } from 'ionic-angular';
import { AddNodePage } from '../add-node/add-node'
import { NodeServiceProvider } from '../../providers/node-service/node-service';

@Component({
  selector: 'page-home',
  templateUrl: 'home.html'
})
export class HomePage {

  nodes: any;

  temperature: any;
  humidity: any;
  smoke: any;
  flame: any;

  nodeList: Array<{
    name: any;
    temperature: any;
    humidity: any;
    smoke: any;
    flame: any;
  }> = [];

  constructor(public navCtrl: NavController, public nodeServiceProvider: NodeServiceProvider) {
    this.getNodes();
  }

  getNodes() {
    //console.log('getting nodes');
    this.nodeServiceProvider.getNodes().then(data => {
      //console.log('got data');
      this.nodes = data;
      //console.log(data);
    }, err => {
      //console.log('got error');
      //console.log(err);
    });
  }

  createNode() {
    this.navCtrl.push(AddNodePage);
    console.log("hi");
  }

  doRefresh(refresher) {
    console.log('Begin async operation', refresher);
    this.getNodes();
    setTimeout(() => {
        console.log('Async operation has ended');
        refresher.complete();
    }, 1500);
  }

  setTemperature(temperature) {
    if (temperature > -50 && temperature < 200) {
      this.temperature = temperature;
      return true;
    }
    else {
      this.temperature = 'NaN'
      return false;
    }
  }

  setHumidity(humidity) {
    if (humidity >= 0 && humidity <= 100) {
      this.humidity = humidity;
      return true;
    }
    else {
      this.humidity = 'NaN'
      return false;
    }
  }

  setSmoke(smoke) {
    if (smoke >= -100 && smoke <= 100) {
      this.smoke = smoke;
      return true;
    }
    else {
      this.smoke = 'NaN'
      return false;
    }
  }

  setFlame(flame) {
    if (flame >= -100 && flame <= 100) {
      this.flame = flame;
      return true;
    }
    else {
      this.flame = 'NaN'
      return false;
    }
  }


}
