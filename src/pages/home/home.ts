import { Component,Injectable } from '@angular/core';
import { Platform, NavController, NavParams,MenuController,LoadingController, ToastController,ModalController, AlertController } from 'ionic-angular';

import { NativeStorage } from '@ionic-native/native-storage';
import { HTTP } from '@ionic-native/http';
import { BluetoothSerial } from '@ionic-native/bluetooth-serial';

import { EventLoggerProvider } from '../../providers/event-logger/event-logger';

import * as Environment from '../../pages/environment.constants';
import { User } from '../../pages/userdata.constants';

import { LoginPage } from '../../pages/login/login';
import { AceitePage } from '../../pages/aceite/aceite';

/**
 * Generated class for the HomePage page.
 *
 * See https://ionicframework.com/docs/components/#navigation for more info on
 * Ionic pages and navigation.
 */


@Component({
	selector: 'page-home',
	templateUrl: 'home.html',
})
@Injectable()
export class HomePage {

	bluetooth_tentativa:any = 0;
	usuario:any;
	bike:any=0;
	write:boolean=false;
	done:boolean=false;
	loader:any;

	historico: any[] = [];

	post_tentativa:any= 0;
	post_tentativa_status:any= 0;

	constructor(
		platform: Platform,
		public navCtrl: NavController,
		public navParams: NavParams,
		private nativeStorage: NativeStorage,
		private menuCtrl: MenuController,
		private http: HTTP,
		private bluetoothSerial: BluetoothSerial,
		public logger: EventLoggerProvider,
		private loadingCtrl: LoadingController,
		private toastCtrl: ToastController,
		private modalCtrl: ModalController,
		public alertCtrl: AlertController,
		public user: User
	) {

		platform.ready().then(() => {
			this.usuario=user;
			this.doVerificaStatus();
		});

	}

	ionViewDidLoad() {
		console.log('ionViewDidLoad HomePage');

		let view = this.navCtrl.getActive();
		this.logger.logScreen(view.component.name);
	}

	ionViewWillEnter() { 
		console.log('ionViewWillEnter HomePage');
		this.menuCtrl.enable(true);
	}

	deslogar() {

		this.nativeStorage.clear();

		this.navCtrl.push(LoginPage).then(() => {
			const index = this.navCtrl.getActive().index;
			this.navCtrl.remove(index-1);
		});

	}

	doAceite() {
		this.logger.logButton('doAceite',{ pram: "Aceite" });
		const modal = this.modalCtrl.create(AceitePage);
		modal.present();
	}

	doVerificaStatus(){

		let url = Environment.API_URL_PROCESS + 'v1/bikes/status';
		console.log('Consultando status...', url);

		this.http.post(url, {request:{cpf:this.user.cpf}}, {})
			.then(data => {

				let obj = JSON.parse(data.data);

				console.log('retorno de consultando status...', obj);
					
				if(obj.result != "SUCCESS"){

					if(this.post_tentativa_status<2){
						this.post_tentativa_status++;
						console.log('post_tentativa_status no success',this.post_tentativa_status);
						setTimeout(() => this.doVerificaStatus(), 3500);
					}else{
						this.post_tentativa_status=0;
						this.showLongToast(Environment.BIKE_ERR + Environment.LOGAR_EQUIPE);
					}
					
				}else{

					if(obj.bike !=undefined){
						console.log('bike:', obj.bike.codigo);
						this.bike = obj.bike.codigo;
					}

					this.storageAceiteSilent(true, this.bike);
					
				}

			})
			.catch(error => {

				console.log(error);
				if(this.post_tentativa_status<2){
					this.post_tentativa_status++;
					console.log('post_tentativa_status no success',this.post_tentativa_status);
					setTimeout(() => this.doVerificaStatus(), 3500);
				}else{
					this.post_tentativa_status=0;
				}

			});

	}

  storageAceiteSilent(val,bike) {

    let novo = { usuario: {
      cpf: this.user.cpf,
      nome: this.user.nome,
      email: this.user.email,
      bike: this.bike,
      aceite: val
    }}
    this.user.aceite = val;

    console.log("Login a atualizar:", novo);

    this.nativeStorage.setItem('login', novo)
    .then((res) => {
      console.log("Updated item!", res);
      
    })
    .catch((error: any) => {
      console.error('Error storing item', error)
    })
    ;

  }

	doVerificaBikes(){

		this.loader = this.loadingCtrl.create({
			content: Environment.BIKE_GET
		});
		this.loader.present();

		this.logger.logButton('doVerificaBikes',{ pram: "VerificaBikes" });

		let url = Environment.API_URL_PROCESS + 'v1/bikes/post';
		console.log('Consultando status...', url);

		this.http.post(url, {request:{cpf:this.usuario.cpf}}, {})
			.then(data => {

				let obj = JSON.parse(data.data);
				console.log('body',obj);

				this.loader.dismiss();
					
				if(obj.result != "SUCCESS"){

					this.showLongToast(Environment.BIKE_ERR + Environment.LOGAR_EQUIPE);
					
				}else{

					this.bike = obj.bike;
					this.showConfirm();
					
				}

			})
			.catch(error => {

				this.loader.dismiss();
				console.log(error);
				this.showLongToast(Environment.BIKE_ERR + Environment.LOGAR_EQUIPE);

			});

	}

	doDesbloqueia(){

		this.loader = this.loadingCtrl.create({
			content: Environment.BIKE_UNLOCK
		});
		this.loader.present();

		this.verifyBluetooth()
			.then((data)=> {
				console.log('verifyBluetooth data')
				console.log(data)

				if(data){//data!=undefined && //!='OK' 
					this.bluetooth_tentativa ++;
					console.log('bluetooth_tentativa',this.bluetooth_tentativa)
					this.connectBluetooth();
				}else{
					this.loader.dismiss();
					this.showLongToast(Environment.BT_INCORRECT_RETURN + Environment.LOGAR_EQUIPE);
					return false;
				}
			})
	}

	doList() {

		let xtest = this.verifyBluetooth();
		console.log('xtest',xtest);
		this.listBluetooth();

	}

	listBluetooth() {

		return this.bluetoothSerial.list()
			.then((ok)=> {console.log("Bluetooth List",ok); return true;})
			.catch((err) => {
				console.log("Bluetooth list error",err);
				//FIREBASE LOG
				return false;
			})
		;

	}


	verifyBluetooth() {

		return this.bluetoothSerial.enable()
			.then((ok)=> {console.log("PASSO 1 - Bluetooth enable",ok); return true;})
			.catch((err) => {
				console.log("PASSO 1 - Bluetooth enable error",err);
				//FIREBASE LOG
				return false;
			})
		;

	}

	disconnectBluetooth() {

		return this.bluetoothSerial.disconnect()
			.then((ok)=> {
				console.log("PASSO FINAL - disconnect do Bluetooth",ok);
				return true;
			})
			.catch((err) => {
				console.log("PASSO FINAL - disconnect do Bluetooth error",err);
				//FIREBASE LOG
				return false;
			})
		;

	}
	doAcionaTrava(bike) { //nao usando parametro mais

		this.loader = this.loadingCtrl.create({
			content: 'Liberando Bike ' +this.bike.toString()+ '. Aguarde...'
		});
		this.loader.present();

		console.log('acionando trava..', this.bike.toString())

		this.bluetoothSerial.connectInsecure(Environment.BT_INCORRECT_HASH)
		.subscribe(
			(ok) => {
				console.log('PASSO 2 - Bluetooth connectInsecure',ok);

				this.bluetoothSerial.write(this.bike.toString()).then((ok)=> {
					console.log('write '+this.bike.toString(),ok);
					this.write = true;
					this.loader.dismiss();

					this.disconnectBluetooth().then((ok)=>{

						this.done = true;
						this.showLongToast(Environment.DONE);
						
					})

				})


			},
			(err) => {
				console.log('PASSO 2 - Bluetooth connectInsecure err',err);
				if(this.bluetooth_tentativa<2){
					this.bluetooth_tentativa++;
					console.log('bluetooth_tentativa',this.bluetooth_tentativa);
					setTimeout(() => this.connectBluetooth(), 3500);
				}else{
					this.bluetooth_tentativa=0;
					this.loader.dismiss();
					this.showLongToast(Environment.BT_WAIT);
				}
			}
		);

	}

	connectBluetooth() {


		this.bluetoothSerial.connectInsecure(Environment.BT_INCORRECT_HASH)
		.subscribe(
			(ok) => {
				console.log('PASSO 2 - Bluetooth connectInsecure',ok);

				this.bluetoothSerial.write(this.bike.toString()).then((ok)=> {
					console.log('write '+this.bike,ok);
					this.write = true;
					this.loader.dismiss();

					this.loader.dismiss();
					this.disconnectBluetooth().then((ok)=>{

						this.done = true;
						this.showLongToast(Environment.DONE);
						
					})

				})


			},
			(err) => {
				console.log('PASSO 2 - Bluetooth connectInsecure err',err);
				if(this.bluetooth_tentativa<2){
					this.bluetooth_tentativa++;
					console.log('bluetooth_tentativa',this.bluetooth_tentativa);
					setTimeout(() => this.connectBluetooth(), 3500);
				}else{
					this.bluetooth_tentativa=0;
					this.loader.dismiss();
					this.showLongToast(Environment.BT_WAIT);
				}
			}
		);

	}

	do121() {

		this.bluetoothSerial.write('marcus').then((ok)=> {

			console.log('enviado a', ok);

			this.bluetoothSerial.available().then((numBytes)=> {

				console.log('numBytes',numBytes);

				this.bluetoothSerial.read().then((data)=> {

					console.log('retornou', data);
					//this.disconectar();

				}).catch((err) => {

					//this.disconectar();
					console.error('read err', err);

				});

			}).catch((err) => {

				//this.disconectar();
				console.error('availableou err', err);

			});

		}).catch((err) => {

			//this.disconectar();
			console.error('do121 err', err);

		});

	}

	showConfirm() {
		const confirm = this.alertCtrl.create({
			title: Environment.BIKE_TITLE + this.bike,
			message: Environment.BIKE_MSG_PRE + this.bike + Environment.BIKE_MSG_AFTER,
			buttons: [
				{
					text: Environment.BIKE_CANCEL,
					handler: () => {
						this.logger.logButton('showConfirm',{ pram: "BIKE_CANCEL" });
						console.log('BIKE_CANCEL clicked');
					}
				},
				{
					text: Environment.BIKE_CONFIRM,
					handler: () => {
						this.logger.logButton('showConfirm',{ pram: "BIKE_CONFIRM" });
						console.log('BIKE_CONFIRM clicked');
						this.confirmaRetirada();
					}
				}
			]
		});
		confirm.present();
	}

	showDevolucao() {
		const confirm = this.alertCtrl.create({
			title: 'Devolver Bike ' + this.bike,
			message: 'Confirmar a devolução da Bike nr ' + this.bike + '?',
			buttons: [
				{
					text: Environment.BIKE_CANCEL,
					handler: () => {
						this.logger.logButton('showDevolucao',{ pram: "BIKE_CANCEL" });
						console.log('BIKE_CANCEL clicked');
					}
				},
				{
					text: Environment.BIKE_CONFIRM,
					handler: () => {
						this.logger.logButton('showDevolucao',{ pram: "BIKE_CONFIRM" });
						console.log('BIKE_CONFIRM clicked');
						this.doConfirmaDevolucao();
					}
				}
			]
		});
		confirm.present();
	}

	confirmaRetirada() {

		this.loader = this.loadingCtrl.create({
			content: Environment.DONE_BIKE
		});
		this.loader.present();

		this.logger.logButton('confirmaRetirada',{ pram: "Retirada" });

		let url = Environment.API_URL_PROCESS + 'v1/bikes/get';
		console.log('Post status...', url);

		this.http.post(url, {request:{cpf:this.usuario.cpf,bike:this.bike}}, {})
			.then(data => {

				let obj = JSON.parse(data.data);
				console.log('body',obj);

				this.loader.dismiss();
					
				if(obj.result != "SUCCESS"){

					if(this.post_tentativa<2){
						this.post_tentativa++;
						console.log('post_tentativa no success',this.post_tentativa);
						setTimeout(() => this.confirmaRetirada(), 3500);
					}else{
						this.post_tentativa=0;
						this.loader.dismiss();
						this.showLongToast(Environment.DONE_ABORT + Environment.LOGAR_EQUIPE);
					}

				}else{

					this.storageAceiteSilent(true,this.bike);
					this.doDesbloqueia();

				}

			})
			.catch(error => {

					console.log(error);

					if(this.post_tentativa<2){
						this.post_tentativa++;
						console.log('post_tentativa err',this.post_tentativa);
						setTimeout(() => this.confirmaRetirada(), 3500);
					}else{
						this.post_tentativa=0;
						this.loader.dismiss();
						console.log(error);
						this.showLongToast(Environment.DONE_ERR + Environment.LOGAR_EQUIPE);
					}


			});
		
	}

	doConfirmaDevolucao() {

		this.loader = this.loadingCtrl.create({
			content: 'Confirmando devolução Bike '+this.bike.toString()+'. Aguarde...'
		});
		this.loader.present();

		this.logger.logButton('confirmaDevolucao',{ pram: "Devolucao" });

		let url = Environment.API_URL_PROCESS + 'v1/bikes/leave';
		console.log('Leave status...', url);

		this.http.post(url, {request:{cpf:this.usuario.cpf,bike:this.bike}}, {})
			.then(data => {

				let obj = JSON.parse(data.data);
				console.log('body',obj);

				this.loader.dismiss();
					
				if(obj.result != "SUCCESS"){

					if(this.post_tentativa<2){
						this.post_tentativa++;
						console.log('post_tentativa no success',this.post_tentativa);
						setTimeout(() => this.doConfirmaDevolucao(), 3500);
					}else{
						this.post_tentativa=0;
						this.loader.dismiss();
						this.showLongToast(Environment.DEVOLUTION_ABORT + Environment.LOGAR_EQUIPE);
					}

				}else{

					this.bike = 0;
					this.storageAceiteSilent(true,this.bike);
					this.showLongToast(Environment.DEVOLUTION);

				}

			})
			.catch(error => {

					console.log(error);

					if(this.post_tentativa<2){
						this.post_tentativa++;
						console.log('post_tentativa err',this.post_tentativa);
						setTimeout(() => this.doConfirmaDevolucao(), 3500);
					}else{
						this.post_tentativa=0;
						this.loader.dismiss();
						console.log(error);
						this.showLongToast(Environment.DEVOLUTION_ERR + Environment.LOGAR_EQUIPE);
					}


			});
		
	}

	showLongToast(message) {
		const toast = this.toastCtrl.create({
			message: message,
			duration: 5000,
		});
		toast.present();
	}

}
