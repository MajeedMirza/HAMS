import { Component } from '@angular/core';
import { NavController, NavParams } from 'ionic-angular';
import { NodeServiceProvider } from '../../providers/node-service/node-service';

@Component({
  selector: 'page-garage',
  templateUrl: 'garage.html',
})
export class GaragePage {

  constructor(public navCtrl: NavController, public navParams: NavParams) {
  }

  ionViewDidLoad() {
    console.log('ionViewDidLoad GaragePage');
  }

}
