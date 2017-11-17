import { NodeServiceProvider } from '../../providers/node-service/node-service'
import { HomePage } from './home'
import { NavController } from 'ionic-angular';
import { NavControllerMock } from 'ionic-mocks';
//import { Http } from '@angular/http';
import {
    TestBed,
    getTestBed,
    async,
    inject
} from '@angular/core/testing';
import {
    Headers, BaseRequestOptions,
    Response, HttpModule, Http, XHRBackend, RequestMethod
} from '@angular/http';

import { ResponseOptions } from '@angular/http';
import { MockBackend, MockConnection } from '@angular/http/testing';

describe('Testing home', () => {

    let mockBackend: MockBackend;
    let navCtrl: NavController;

    beforeEach(async(() => {
        TestBed.configureTestingModule({
            providers: [
                NodeServiceProvider,
                MockBackend,
                BaseRequestOptions,
                {
                    provide: Http,
                    deps: [MockBackend, BaseRequestOptions],
                    useFactory:
                    (backend: XHRBackend, defaultOptions: BaseRequestOptions) => {
                        return new Http(backend, defaultOptions);
                    }
                }
            ],
            imports: [
                HttpModule
            ]
        });
        mockBackend = getTestBed().get(MockBackend);
    }));

    it('Good values case', async(inject([NodeServiceProvider], (service) => {
        mockBackend.connections.subscribe(
            (connection: MockConnection) => {
                connection.mockRespond(new Response(
                    new ResponseOptions({
                        body: [
                            {
                                temperature: 12,
                                humidity: 34,
                                smoke: 0,
                                flame: 0,
                            }]
                    }
                    )));
            });

        service.getNodes().then(
            (data) => {
                expect(data.length).toBe(1);
                expect(data[0].temperature).toBeLessThanOrEqual(200);
                expect(data[0].temperature).toBeGreaterThanOrEqual(-50);
                expect(data[0].humidity).toBeLessThanOrEqual(100);
                expect(data[0].humidity).toBeGreaterThanOrEqual(0);
                expect(data[0].smoke).toBeLessThanOrEqual(100);
                expect(data[0].smoke).toBeGreaterThanOrEqual(-100);
                expect(data[0].flame).toBeLessThanOrEqual(100);
                expect(data[0].flame).toBeGreaterThanOrEqual(-100);
            });
    }))
    
    );

    it ('Bad Server Request', () => {
        let http: Http;
        let service: NodeServiceProvider;
        service = new NodeServiceProvider(http)
        let home: HomePage;
        home = new HomePage(navCtrl, service);
        expect(
            home.getNodes()
        ).toBeFalsy();
    });

    it('Create Node with valid values', async(inject([NodeServiceProvider], (service) => {
        expect(function() {service.createNode('name', 'id', 'type', 'location')}).not.toThrow();
    })))

    it('Create Node with empty value', async(inject([NodeServiceProvider], (service) => {
        expect(function() {service.createNode()}).toThrow();
    })))

    it('Get Node Values with Valid ID', async(inject([NodeServiceProvider], (service) => {
        expect(function() {service.getNodeValues('ID')}).not.toThrow();
    })))
    
    it('Get Node Values with Empty ID', async(inject([NodeServiceProvider], (service) => {
        expect(function() {service.getNodeValues()}).toThrow();
    })))

    it ('Set Valid Node Values', () => {
        let http: Http;
        let service: NodeServiceProvider;
        service = new NodeServiceProvider(http)
        let home: HomePage;
        home = new HomePage(navCtrl, service);
        expect(
            home.setTemperature(25)
        ).toBeTruthy();
        expect(
            home.setHumidity(35)
        ).toBeTruthy();
        expect(
            home.setSmoke(1)
        ).toBeTruthy();
        expect(
            home.setFlame(25)
        ).toBeTruthy();
    });

    it ('Set Invalid Node Values', () => {
        let http: Http;
        let service: NodeServiceProvider;
        service = new NodeServiceProvider(http)
        let home: HomePage;
        home = new HomePage(navCtrl, service);
        expect(
            home.setTemperature(3000)
        ).toBeFalsy();
        expect(
            home.setHumidity(-20)
        ).toBeFalsy();
        expect(
            home.setSmoke(1200)
        ).toBeFalsy();
        expect(
            home.setFlame(-200)
        ).toBeFalsy();
    });
}) 