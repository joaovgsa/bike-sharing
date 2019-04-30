import { Component } from '@angular/core';
import { Platform, NavController, NavParams, ToastController,ViewController, LoadingController } from 'ionic-angular';

import { NativeStorage } from '@ionic-native/native-storage';
import { HTTP } from '@ionic-native/http';

import { EventLoggerProvider } from '../../providers/event-logger/event-logger';

import * as Environment from '../../pages/environment.constants';
import { User } from '../../pages/userdata.constants';

/**
 * Generated class for the AceitePage page.
 *
 * See https://ionicframework.com/docs/components/#navigation for more info on
 * Ionic pages and navigation.
 */


@Component({
  selector: 'page-aceite',
  templateUrl: 'aceite.html',
})
export class AceitePage {

  post_tentativa:any= 0;
  loader:any;
  usuario:any;

  constructor(
    platform: Platform,
  	public navCtrl: NavController,
  	public navParams: NavParams,
    private viewCtrl: ViewController,
		public logger: EventLoggerProvider,
    private nativeStorage: NativeStorage,
    private toastCtrl: ToastController,
    public user: User,
    private http: HTTP,
    private loadingCtrl: LoadingController,
	) {

    platform.ready().then(() => {
      this.usuario=user;
    });

  }

  ionViewDidLoad() {
    console.log('ionViewDidLoad AceitePage');
  }

  onBackTermos() {
    this.logger.logButton('onBackTermos',{ pram: "BackTermos" });
    this.viewCtrl.dismiss();
  }

  checkAceite() {

    this.loader = this.loadingCtrl.create({
      content: Environment.TERM_ACCEPTING
    });
    this.loader.present();

    this.logger.logButton('checkAceite',{ pram: "Aceite" });

    let url = Environment.API_URL_PROCESS + 'v1/bikes/sign';
    console.log('Post aceite...', url);

    this.http.post(url, {request:{cpf:this.usuario.cpf}}, {})
      .then(data => {

        let obj = JSON.parse(data.data);
        console.log('body',obj);

        this.loader.dismiss();
          
        if(obj.result != "SUCCESS"){

          if(this.post_tentativa<2){
            this.post_tentativa++;
            console.log('post_tentativa no success',this.post_tentativa);
            setTimeout(() => this.checkAceite(), 3500);
          }else{
            this.post_tentativa=0;
            this.loader.dismiss();
            this.showLongToast(Environment.DONE_ABORT + Environment.LOGAR_EQUIPE);
          }

        }else{

          this.storageAceite();

        }

      })
      .catch(error => {

          console.log(error);

          if(this.post_tentativa<2){
            this.post_tentativa++;
            console.log('post_tentativa err',this.post_tentativa);
            setTimeout(() => this.checkAceite(), 3500);
          }else{
            this.post_tentativa=0;
            this.loader.dismiss();
            console.log(error);
            this.showLongToast(Environment.DONE_ERR + Environment.LOGAR_EQUIPE);
          }


      });
    
  }


  storageAceite() {

    let novo = { usuario: {
      cpf: this.user.cpf,
      nome: this.user.nome,
      email: this.user.email,
      aceite: true
    }}
    this.user.aceite = true;

    console.log("Login a atualizar:", novo);

    this.nativeStorage.setItem('login', novo)
    .then((res) => {
      console.log("Updated item!", res);
      this.loader.dismiss();
      this.showLongToast(Environment.TERM_OK);
      this.viewCtrl.dismiss();
      
    })
    .catch((error: any) => {
      console.error('Error storing item', error)
      this.showLongToast(Environment.TERM_ERR);
    })
    ;

  }

  showLongToast(message) {
    const toast = this.toastCtrl.create({
      message: message,
      duration: 5000,
    });
    toast.present();
  }

}
