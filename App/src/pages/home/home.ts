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

  nodes: any; // holds all nodes
  socket:any; // socket for realtime update

  constructor(public navCtrl: NavController, public nodeServiceProvider: NodeServiceProvider) {
    // Fetch node values on start
    this.getNodes();
    // Socket setup for realtime value update
    this.socket = io(nodeServiceProvider.getServer());
    this.socket.on('values', (node) => {
      for (let i in this.nodes) {
        if (this.nodes[i].id == node.id) {
          this.nodes[i].values = node.values;
        }
      }
    });
  }

  createNode() {
    // Open AddNodePage to create a node
    this.navCtrl.push(AddNodePage);
  }

  getNodes() {
    // call nodeServiceProvider getNodes() function to get all nodes
    this.nodeServiceProvider.getNodes().then(data => {
      this.nodes = data;
    }, err => {
      // warn user about the error
      alert(err);
    });
  }

  // Pull down to refresh the data
  doRefresh(refresher) {
    this.getNodes();
    setTimeout(() => {
        refresher.complete();
    }, 1500);
  }
}
