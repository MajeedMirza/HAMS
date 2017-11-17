import { NgModule } from '@angular/core';
import { IonicPageModule } from 'ionic-angular';
import { GaragePage } from './garage';

@NgModule({
  declarations: [
    GaragePage,
  ],
  imports: [
    IonicPageModule.forChild(GaragePage),
  ],
})
export class GaragePageModule {}
