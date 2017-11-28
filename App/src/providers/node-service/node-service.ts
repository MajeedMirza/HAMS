import { Injectable } from '@angular/core';
import { Http } from '@angular/http';
import 'rxjs/add/operator/map';

@Injectable()
export class NodeServiceProvider {

    // Server IP Address
    server = 'http://172.17.74.11:3001';

    constructor(public http: Http) {

    }
    
    createNode(name: string, id: string, type: string, location: string) {
        var node = {
            name: name,
            id: id,
            type: type,
            location: location
        }
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
                    resolve({data}.data);
                },
                err => {
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
                    resolve(data);
                },
                err => {
                    reject(err);
                }
            )
        });
    }

    openGarage() {
        this.http.get(this.server + '/api/nodes/garage/true').subscribe(res => {
            
        }, err => {
            alert("Cannot open garage");
        })
    }

    closeGarage() {
        this.http.get(this.server + '/api/nodes/garage/false').subscribe(res => {

        }, err => {
            alert("Cannot close garage");
        })
    }

    setServer(ip) {
        this.server = "http://" + ip + ":3001";
    }

    getServer() {
        return this.server;
    }
}
