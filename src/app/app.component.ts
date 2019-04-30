import { Component,Injectable } from '@angular/core';
import { Platform } from 'ionic-angular';
import { StatusBar } from '@ionic-native/status-bar';
import { SplashScreen } from '@ionic-native/splash-screen';
import { NativeStorage } from '@ionic-native/native-storage';

//import { AnalyticsFirebase } from '@ionic-native/analytics-firebase';

import { LoginPage } from '../pages/login/login';
import { HomePage } from '../pages/home/home';

import { User } from '../pages/userdata.constants';

@Component({
  templateUrl: 'app.html'
})
@Injectable()
export class MyBikeSharing {
  rootPage:any;

  constructor(
    platform: Platform,
    statusBar: StatusBar,
    splashScreen: SplashScreen,
    private nativeStorage: NativeStorage,
    public user: User
    //private analyticsFirebase: AnalyticsFirebase,
  ) {
    platform.ready().then(() => {
      // Okay, so the platform is ready and our plugins are available.
      // Here you can do any higher level native things you might need.
      statusBar.styleDefault();
      statusBar.overlaysWebView(true);
      statusBar.styleLightContent();
      statusBar.backgroundColorByHexString('#ee8322');
      splashScreen.hide();
      statusBar.hide();
      
      this.doLoadData();

    });
  }

  doLoadData() {

    this.nativeStorage.getItem('login')
      .then((res) => {
        console.log("Getttted item!", res);

        this.user.cpf = res.usuario.cpf;
        this.user.nome = res.usuario.name;
        this.user.email = res.usuario.email;
        this.user.aceite = res.usuario.aceite;
        

        // this.analyticsFirebase.setUserId(res.usuario.cpf)
        //   .then(() => console.log('User id successfully set'))
        //   .catch(err => console.log('Error setting user id:', err));

        // this.analyticsFirebase.setCurrentScreen('App')
        //   .then(() => console.log('View successfully tracked'))
        //   .catch(err => console.log('Error tracking view:', err));

        this.rootPage = HomePage;
      })
      .catch((error: any) => {
        console.error('Error retriving item!', error);
        this.rootPage = LoginPage;
      })
    ;

  }
}

