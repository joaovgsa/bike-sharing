import { Injectable } from '@angular/core';

@Injectable()
export class User {
  public cpf: string = '';
  public nome: string = '';
  public email: string = '';
  public aceite: boolean = false;
}