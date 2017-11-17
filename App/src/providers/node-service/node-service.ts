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

    }
    // TODO: replace it once the server is ready
    server = 'http://localhost:3001';

    createNode(name: string, id: string, type: string, location: string) {
        var node = {
            name: name,
            id: id,
            type: type,
            location: location
        }
        if (!name || !id || !type || !location) {
            throw  'Invalid Input';
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
                   // console.log('Got nodes successfully');
                    resolve({data}.data);
                },
                err => {
                    //console.log("Failed to get nodes.");
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
        if (!nodeID) {
            throw 'Invalid ID';
        }
        return new Promise((resolve, reject) => {
            this.http.get(this.server + '/api/logs/getlatest/' + nodeID).map(res => res.json()).subscribe(
                data => {
                    //console.log('Got nodes successfully');
                    resolve(data);
                },
                err => {
                    //console.log("Failed to get nodes.");
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
