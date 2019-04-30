import { Component } from '@angular/core';
import { NavController, NavParams, ViewController, LoadingController, ToastController } from 'ionic-angular';

import { Validators, FormBuilder, FormGroup, FormControl } from '@angular/forms'

import { InAppBrowser } from '@ionic-native/in-app-browser';
import { HTTP } from '@ionic-native/http';
import { NativeStorage } from '@ionic-native/native-storage';
import { EventLoggerProvider } from '../../providers/event-logger/event-logger';

import { HomePage } from '../../pages/home/home';

import * as Environment from '../../pages/environment.constants';
import { User } from '../../pages/userdata.constants';

/**
 * Generated class for the LoginModalPage page.
 *
 * See https://ionicframework.com/docs/components/#navigation for more info on
 * Ionic pages and navigation.
 */


@Component({
	selector: 'page-login-modal',
	templateUrl: 'login-modal.html',
})
export class LoginModalPage {

	private frmLogin : FormGroup;
	private loader : any;
	usuario:any;
	post_tentativa:any= 0;

	constructor(
		public navCtrl: NavController,
		public navParams: NavParams,
		private viewCtrl: ViewController,
		private iab: InAppBrowser,
		private formBuilder: FormBuilder,
		private http: HTTP,
		private nativeStorage: NativeStorage,
		private loadingCtrl: LoadingController,
		private toastCtrl: ToastController,
		public user: User,
		public logger: EventLoggerProvider,

	) {

		this.frmLogin = this.formBuilder.group({
			fieldCPF: new FormControl('', Validators.compose([
				Validators.required,
				Validators.minLength(11),//11
				Validators.maxLength(11),//11
			])),
			fieldSenha: new FormControl('', Validators.compose([
				Validators.required,
				Validators.minLength(4),//4
				Validators.maxLength(50)//50
			]))
		});

	}

	ionViewDidLoad() {
		console.log('ionViewDidLoad LoginModalPage');
	}

	dismiss() {
		this.viewCtrl.dismiss();
	}

	forgot() {

		let url = 'https://tardis.madeiramadeira.com.br/access/password/reset';
		let target = '_system';

		this.iab.create(url,target);

	}

	logForm(){

		this.loader = this.loadingCtrl.create({
			content: Environment.LOGAR_EFETUANDO
		});
		this.loader.present();

		let url = Environment.API_URL_CORE + 'access/sign-in/' +  Environment.pubKey;
		//console.log('Logando...', url);

		interface iDados{
			cpf:string;
			password:string;
		}
		let usuario = {} as iDados;
		usuario.cpf = this.frmLogin.value.fieldCPF;
		usuario.password = this.frmLogin.value.fieldSenha;

		interface iRequest{ request:object; }
		let request = {} as iRequest;

		request.request = usuario;

		console.log('request',request);

		this.http.post(url, request, {})
			.then(data => {

				console.log(data.status);
				console.log(data.data); // data received by server
				console.log(data.headers);

				let obj = JSON.parse(data.data);
				obj = obj.response;
				console.log('body',obj);

				this.loader.dismiss();
					
				if(obj.result != "SUCCESS"){

					this.showLongToast(Environment.LOGAR_WARNING + Environment.LOGAR_EQUIPE);
					this.nativeStorage.clear();

				}else{

					this.nativeStorage.setItem('login', obj)
					.then((res) => {
						console.log("Stored item!", res);

						// var verificaStatus = this.doVerificaStatus();
						
						// console.log("Passou do verificando!", res);
						// res.usuario.aceite = verificaStatus;
						// console.log('verificaStatus',verificaStatus);

						this.storeUserData(res);
						
						this.viewCtrl.dismiss();
						this.navCtrl.push(HomePage, obj);
						
					})
					.catch((error: any) => {
						console.error('Error storing item', error)
					})
					;

				}

			})
			.catch(error => {

				this.loader.dismiss();

				this.showLongToast(Environment.LOGAR_ERRO + Environment.LOGAR_EQUIPE);

				//FIREBASE LOGAR ERRO CRITICO

				this.nativeStorage.clear();

				console.log(error.status);
				console.log(error.error); // error message as string
				console.log(error.headers);

			});

	}



	storeUserData(res){

		this.user.cpf = res.usuario.cpf;
		this.user.nome = res.usuario.name;
		this.user.email = res.usuario.email;
		this.user.aceite = res.usuario.aceite;

	}

	showLongToast(message) {
		const toast = this.toastCtrl.create({
			message: message,
			duration: 5000,
		});
		toast.present();
	}

}
