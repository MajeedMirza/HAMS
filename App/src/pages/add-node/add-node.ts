import { Component } from '@angular/core';
import { IonicPage, NavController, NavParams } from 'ionic-angular';

import { NodeServiceProvider } from '../../providers/node-service/node-service';

/**
 * Generated class for the AddNodePage page.
 *
 * See https://ionicframework.com/docs/components/#navigation for more info on
 * Ionic pages and navigation.
 */

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
    this.nodeServiceProvider.createNode(this.name, this.id, this.type, this.location).then(() => {
      this.navCtrl.pop();
    })
  }

  
}
