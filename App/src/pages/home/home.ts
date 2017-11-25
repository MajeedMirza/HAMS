import { Component } from '@angular/core';
import { NavController } from 'ionic-angular';
import { AddNodePage } from '../add-node/add-node'
import { NodeServiceProvider } from '../../providers/node-service/node-service';
import * as io from 'socket.io-client';

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

  socket:any;

  constructor(public navCtrl: NavController, public nodeServiceProvider: NodeServiceProvider) {
    this.getNodes();
    this.socket = io(nodeServiceProvider.getServer());

    this.socket.on('values', (node) => {
      console.log("message", node);
      // TODO Live update values
      for (let i in this.nodes) {
        if (this.nodes[i].id == node.id) {
          console.log("hi");
          this.nodes[i].values = node.values;
        }
      }
    });

  }

  createNode() {
    this.navCtrl.push(AddNodePage);
    console.log("hi");
  }

  getNodes() {
    this.nodeServiceProvider.getNodes().then(data => {
      this.nodes = data;
      console.log(data);
    }, err => {
      console.log(err);
    });
  }

  doRefresh(refresher) {
    console.log('Begin async operation', refresher);
    this.getNodes();
    setTimeout(() => {
        console.log('Async operation has ended');
        refresher.complete();
    }, 1500);
  }
}
