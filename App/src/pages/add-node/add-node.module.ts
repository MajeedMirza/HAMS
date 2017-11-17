import { NgModule } from '@angular/core';
import { IonicPageModule } from 'ionic-angular';
import { AddNodePage } from './add-node';

@NgModule({
  declarations: [
    AddNodePage,
  ],
  imports: [
    IonicPageModule.forChild(AddNodePage),
  ],
  exports: [
    AddNodePage,
  ]
})
export class AddNodePageModule {}
