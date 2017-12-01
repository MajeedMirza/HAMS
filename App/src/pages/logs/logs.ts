import { Component } from '@angular/core';
import { NavController, NavParams } from 'ionic-angular';
import { NodeServiceProvider } from '../../providers/node-service/node-service';

import moment from 'moment';

@Component({
  selector: 'page-logs',
  templateUrl: 'logs.html',
})
export class LogsPage {

  logs: any;
  times: any;

  constructor(public navCtrl: NavController, 
    public navParams: NavParams,
    public nodeServiceProvider: NodeServiceProvider) {
    this.getLogs();
  }

  getLogs() {
    this.nodeServiceProvider.getLogs().then((data) => {
      this.logs = data;
      this.logs.reverse();
      this.times = moment(this.logs.values.time);
    }, (err) =>{
      console.log(err);
    })
  }

  getTime(time) {
    return moment(time).format('MMMM Do, h:mm:ss a');
  }

  doRefresh(refresher) {
    this.getLogs();
    setTimeout(() => {
        refresher.complete();
    }, 1500);
  }

  ionViewDidLoad() {
    
  }

}
