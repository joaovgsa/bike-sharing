import { Injectable } from '@angular/core';
//import { FirebaseAnalytics } from '@ionic-native/firebase-analytics';

/*
  Generated class for the EventLoggerProvider provider.

  See https://angular.io/guide/dependency-injection for more info on providers
  and Angular DI.
*/
@Injectable()
export class EventLoggerProvider {

  constructor(
  	//public fba: FirebaseAnalytics
  ) {
    console.log('Hello EventLoggerProvider Provider');
  }

  logButton(name:string,value:any){
    // this.fba.logEvent(name, { pram:value })
    // .then((res: any) => {console.log('Event successfully tracked ' + name + '{'+value+'}', res);})
    // .catch((error: any) => console.log('Error tracking event ' + name + '{'+value+'}', error));
  }

  logScreen(name:string){
    // this.fba.setCurrentScreen(name)
    //   .then(() => console.log('View successfully tracked ' + name, res))
    //   .catch(err => console.log('Error tracking view ' + name, err));
  }

}
