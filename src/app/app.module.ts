import { enableProdMode } from '@angular/core';
enableProdMode();

import { BrowserModule } from '@angular/platform-browser';
import { ErrorHandler, NgModule,Injectable } from '@angular/core';
import { IonicApp, IonicErrorHandler, IonicModule } from 'ionic-angular';
import { SplashScreen } from '@ionic-native/splash-screen';
import { StatusBar } from '@ionic-native/status-bar';

import { NativeStorage } from '@ionic-native/native-storage';
import { InAppBrowser } from '@ionic-native/in-app-browser';
import { HTTP } from '@ionic-native/http';
import { BluetoothSerial } from '@ionic-native/bluetooth-serial';

//import { FirebaseAnalytics } from '@ionic-native/firebase-analytics';

import { MyBikeSharing } from './app.component';

import { HomePage } from '../pages/home/home';

import { LoginPageModule } from '../pages/login/login.module';
import { LoginModalPage } from '../pages/login-modal/login-modal';
import { AceitePage } from '../pages/aceite/aceite';
import { PrivacidadePage } from '../pages/privacidade/privacidade';

import { EventLoggerProvider } from '../providers/event-logger/event-logger';

import { User } from '../pages/userdata.constants';

@Injectable()
@NgModule({
  declarations: [
    MyBikeSharing,
    HomePage,
    LoginModalPage,
    PrivacidadePage,
    AceitePage
  ],
  imports: [
    BrowserModule,
    IonicModule.forRoot(MyBikeSharing, {statusbarPadding: true}),
    LoginPageModule
  ],
  bootstrap: [IonicApp],
  entryComponents: [
    MyBikeSharing,
    HomePage,
    LoginModalPage,
    PrivacidadePage,
    AceitePage
  ],
  providers: [
    StatusBar,
    SplashScreen,
    {provide: ErrorHandler, useClass: IonicErrorHandler},
    NativeStorage,
    InAppBrowser,
    HTTP,
    BluetoothSerial,
    User,
    //FirebaseAnalytics,
    EventLoggerProvider
  ]
})
export class AppModule {}
