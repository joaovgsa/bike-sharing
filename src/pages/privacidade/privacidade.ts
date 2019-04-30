import { Component } from '@angular/core';
import { NavController, NavParams } from 'ionic-angular';

import { EventLoggerProvider } from '../../providers/event-logger/event-logger';

/**
 * Generated class for the PrivacidadePage page.
 *
 * See https://ionicframework.com/docs/components/#navigation for more info on
 * Ionic pages and navigation.
 */


@Component({
  selector: 'page-privacidade',
  templateUrl: 'privacidade.html',
})
export class PrivacidadePage {

  constructor(
  	public navCtrl: NavController,
  	public navParams: NavParams,
		public logger: EventLoggerProvider,
	) {
  }

  ionViewDidLoad() {
    console.log('ionViewDidLoad PrivacidadePage');
  }

  onBackPrivacidade() {
    this.logger.logButton('onBackPrivacidade',{ pram: "BackPrivacidade" });
    const index = this.navCtrl.getActive().index;
		this.navCtrl.remove(index-1);
  }

}
