import { Component } from '@angular/core';
import { NavController, NavParams } from 'ionic-angular';

import { NodeServiceProvider } from '../../providers/node-service/node-service';

@Component({
  selector: 'page-add-node',
  templateUrl: 'add-node.html',
})
export class AddNodePage {

  location: any;
  type: any;
  name: any;
  id: any;

  constructor(public navCtrl: NavController, public navParams: NavParams, public nodeServiceProvider: NodeServiceProvider) {
  }

  ionViewDidLoad() {
    console.log('ionViewDidLoad AddNodePage');
  }

  createNode() {
    this.nodeServiceProvider.createNode(this.name, this.id, this.type, this.location).then((res) => {
      this.navCtrl.pop();
    }, (err) => {
      console.log(err);
    })
  }

  
}
