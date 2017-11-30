import { BrowserModule } from '@angular/platform-browser';
import { ErrorHandler, NgModule } from '@angular/core';
import { IonicApp, IonicErrorHandler, IonicModule } from 'ionic-angular';
import { HttpModule } from '@angular/http';
import { Firebase } from '@ionic-native/firebase';

import { MyApp } from './app.component';
import { HomePage } from '../pages/home/home';
import { AddNodePage } from '../pages/add-node/add-node';
import { GaragePage } from '../pages/garage/garage';
import { ServerPage } from '../pages/server/server';
import { LogsPage } from '../pages/logs/logs'

import { StatusBar } from '@ionic-native/status-bar';
import { SplashScreen } from '@ionic-native/splash-screen';
import { NodeServiceProvider } from '../providers/node-service/node-service';

@NgModule({
  declarations: [
    MyApp,
    HomePage,
    AddNodePage,
    GaragePage,
    ServerPage,
    LogsPage
  ],
  imports: [
    BrowserModule,
    IonicModule.forRoot(MyApp),
    HttpModule
  ],
  bootstrap: [IonicApp],
  entryComponents: [
    MyApp,
    HomePage,
    AddNodePage,
    GaragePage,
    ServerPage,
    LogsPage
  ],
  providers: [
    StatusBar,
    SplashScreen,
    {provide: ErrorHandler, useClass: IonicErrorHandler},
    Firebase,
    NodeServiceProvider
  ]
})
export class AppModule {}
