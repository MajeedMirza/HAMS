import { Component } from '@angular/core';
import { NavController, NavParams } from 'ionic-angular';
import { NodeServiceProvider } from '../../providers/node-service/node-service';

@Component({
  selector: 'page-server',
  templateUrl: 'server.html',
})
export class ServerPage {

  server: any;
  ip: any;

  constructor(public navCtrl: NavController, 
    public navParams: NavParams,
    public nodeServiceProvider: NodeServiceProvider) {
      this.server = nodeServiceProvider.getServer();
  }

  ionViewDidLoad() {
    console.log('ionViewDidLoad ServerPage');
  }

  setServer() {
    this.nodeServiceProvider.setServer(this.ip);
  }

}
