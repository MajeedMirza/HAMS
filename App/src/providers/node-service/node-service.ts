import { Injectable } from '@angular/core';
import { Http } from '@angular/http';
import 'rxjs/add/operator/map';

/*
Generated class for the NodeServiceProvider provider.

See https://angular.io/guide/dependency-injection for more info on providers
and Angular DI.
*/
@Injectable()
export class NodeServiceProvider {

    constructor(public http: Http) {
        console.log('Hello NodeServiceProvider Provider');
    }
    // TODO: replace it once the server is ready
    server = 'http://172.17.88.27:3001';

    createAccount(name: string, email: string, phone: string, address: string, password: string) {
        var user = {
            name: name,
            email: email,
            phone: phone,
            address: address,
            hash: password,
        }
        console.log(user);
        return new Promise((resolve, reject) => {
            this.http.post(this.server + '/api/users/register/', user).subscribe(
                res => {
                    resolve(user);
                },
                err => {
                    reject(err);
                }
            );
        })
    }

    createNode(lat: string, lng: string, type: string, name: string, id: string) {
        var node = {
            lat: lat,
            lng: lng,
            type: type,
            name: name,
            id: id
        }
        //TODO: Add if function to prevent corrupt data
        return new Promise((resolve, reject) => {
            this.http.post(this.server + '/api/nodes/register/', node).subscribe(
                res => {
                    resolve(node);
                },
                err => {
                    reject(err);
                }
            );
        });
    }

    getNodes() {
        return new Promise((resolve, reject) => {
            this.http.get(this.server + '/api/nodes/getAll/').map(res => res.json()).subscribe(
                data => {
                    console.log('Got nodes successfully');
                    resolve({data}.data);
                },
                err => {
                    console.log("Failed to get nodes.");
                    reject(err);
                }
            )
        });
    }

    deleteNode(nodeID) {
        return new Promise((resolve, reject) => {
            this.http.delete(this.server + '/api/nodes/' + nodeID, nodeID).subscribe(
                res => {
                    resolve(nodeID);
                },
                err => {
                    reject(err);
                }
            )
        });
    }

    getNodeValues(nodeID) {
        return new Promise((resolve, reject) => {
            this.http.get(this.server + '/api/logs/getlatest/' + nodeID).map(res => res.json()).subscribe(
                data => {
                    console.log('Got nodes successfully');
                    resolve(data);
                },
                err => {
                    console.log("Failed to get nodes.");
                    reject(err);
                }
            )
        });
    }

    setServer(ip) {
        this.server = "http://" + ip + ":3001";
    }

    getServer() {
        return this.server;
    }
}
