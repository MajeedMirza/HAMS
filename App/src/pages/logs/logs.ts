import { Component } from '@angular/core';
import { NavController, NavParams } from 'ionic-angular';
import { NodeServiceProvider } from '../../providers/node-service/node-service';

@Component({
  selector: 'page-logs',
  templateUrl: 'logs.html',
})
export class LogsPage {

  logs: any;

  constructor(public navCtrl: NavController, 
    public navParams: NavParams,
    public nodeServiceProvider: NodeServiceProvider) {
      nodeServiceProvider.getLogs().then((data) => {
        this.logs = JSON.stringify(data);
      }, (err) =>{

      })
  }

  ionViewDidLoad() {
    
  }

}
