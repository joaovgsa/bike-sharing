import { Component } from '@angular/core';
import { NavController, NavParams, ModalController } from 'ionic-angular';

import { EventLoggerProvider } from '../../providers/event-logger/event-logger';

import { LoginModalPage } from '../../pages/login-modal/login-modal';
import { PrivacidadePage } from '../../pages/privacidade/privacidade';

/**
 * Generated class for the LoginPage page.
 *
 * See https://ionicframework.com/docs/components/#navigation for more info on
 * Ionic pages and navigation.
 */


@Component({
  selector: 'page-login',
  templateUrl: 'login.html',
})
export class LoginPage {

  constructor(
  	public navCtrl: NavController,
  	public navParams: NavParams,
  	private modalCtrl: ModalController,
    public logger: EventLoggerProvider,
  ) {
  }

  ionViewDidLoad() {
    console.log('ionViewDidLoad LoginPage');
    let view = this.navCtrl.getActive();
    this.logger.logScreen(view.component.name);
  }

  ionViewWillEnter() {
    console.log('ionViewWillEnter LoginPage');
  }

  onOpenLogin() {
    this.logger.logButton('onOpenLogin',{ pram: "OpenLogin" });
    const modal = this.modalCtrl.create(LoginModalPage);
    modal.present();
  }

  onOpenPrivacidade() {
    this.logger.logButton('onOpenPrivacidade',{ pram: "OpenPrivacidade" });
    const modal = this.modalCtrl.create(PrivacidadePage);
    modal.present();
  }

}
