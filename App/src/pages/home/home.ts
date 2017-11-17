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
    nodeServiceProvider.getNodes().then(data => {
      this.nodes = data;
      console.log(data);
    }, err => {
      console.log(err);
    });
  }

  createNode() {
    this.navCtrl.push(AddNodePage);
    console.log("hi");
  }

  doRefresh(refresher) {
    console.log('Begin async operation', refresher);
    setTimeout(() => {
        console.log('Async operation has ended');
        refresher.complete();
    }, 1500);
  }


}
