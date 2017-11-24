import { Component, ViewChild } from '@angular/core';
import { Nav, Platform } from 'ionic-angular';
import { StatusBar } from '@ionic-native/status-bar';
import { SplashScreen } from '@ionic-native/splash-screen';
import { Firebase } from '@ionic-native/firebase';

import { HomePage } from '../pages/home/home';
import { GaragePage } from '../pages/garage/garage';
import { ServerPage } from '../pages/server/server';

@Component({
  templateUrl: 'app.html'
})
export class MyApp {
  @ViewChild(Nav) nav: Nav;

  rootPage: any = HomePage;

  pages: Array<{title: string, component: any}>;

  constructor(public platform: Platform, public statusBar: StatusBar, public splashScreen: SplashScreen, public firebase: Firebase) {
    this.initializeApp();
    this.firebase.getToken()
    .then(token => console.log(`The token is ${token}`)) // save the token server-side and use it to push notifications to this device
    .catch(error => {
      console.error('Error getting token', error);
      //alert("Failed to get token.")
    });

    //subscirbe to emergency
    this.firebase.subscribe('emergency').then(res => {
      //alert("Subscirbed to emergency.");
      console.log('subscribed to emergency!');
    }, err => {
      //alert("Failed to subscribe.");
      console.log(err);
    });

    //notification handler
    this.firebase.onNotificationOpen().subscribe(res => {
      console.log('notification opened.');
      //this.firebaseServiceProvider.setData(res);
      //this.showAlert(this.firebaseServiceProvider.getMessage());
    }, err => {
      console.log(err);
    });

    // used for an example of ngFor and navigation
    this.pages = [
      { title: 'Home', component: HomePage },
      { title: 'Garage', component: GaragePage},
      { title: 'Server', component: ServerPage}
    ];

  }

  initializeApp() {
    this.platform.ready().then(() => {
      // Okay, so the platform is ready and our plugins are available.
      // Here you can do any higher level native things you might need.
      this.statusBar.styleDefault();
      this.splashScreen.hide();
    });
  }

  openPage(page) {
    // Reset the content nav to have just this page
    // we wouldn't want the back button to show in this scenario
    this.nav.setRoot(page.component);
  }
}
