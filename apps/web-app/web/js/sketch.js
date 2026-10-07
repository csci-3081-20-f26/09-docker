//var socket = new WebSocket("ws://" + location.hostname+(location.port ? ':'+location.port: ''), "web_server");

let api = new WSApi();

var count = 0;
var size = 80;
var color = {r: 0, g: 0, b: 0, a: 0}
var id;
var connected = false;

let img;
let map;
let robotImg;
let entityImages = {}

// Load the image.
function preload() {
}

try {
	api.onmessage =function got_packet(msg, data) {
		if (!connected) {
			id = +msg.data;
			connected = true;
		}
		count++;
		//console.log(count, JSON.parse(msg.data));
		var data = JSON.parse(msg.data);
		if (data.cmd == "updateElipse") {	
			size = data.size;
			color.r = data.color.r;
			color.g = data.color.g;
			color.b = data.color.b;
			color.a = data.color.a;	
		}
	} 
} catch(exception) {
	alert('<p>Error' + exception);  
}

let doneSetup = false;

// P5 functions
function setup() {
	api.sendCommand("setup", {width: windowWidth-20, height: windowHeight-20}).then(function(data) {
		map = loadImage(data["scene"]["map"]);
		let entities = data["scene"]["entities"]
		for (let i = 0; i < entities.length; i++) {
			entityImages[entities[i]["id"]] = loadImage(entities[i]["image"]);
		}
		createCanvas(data["scene"]["width"], data["scene"]["height"]);
		doneSetup = true;
	});

}

let updateNum = 0;
let updating = false;

function draw() {
  if (!doneSetup) {
	return;
  }
	updating = true;
  	updateNum++;
	api.sendCommand("update", {n: updateNum}).then(function(data) {
		let entities = data["e"];
		for (let i = 0; i < entities.length; i++) {
			entities[i].p[1] = height - entities[i].p[1];
			entities[i].d[1] *= -1.0
		}
		drawAll(entities);
		updating = false;
	});
  
}

function drawAll(entities) {
  background(128); 
  image(map, 0, 0, width, height);
  
  fill(color.r, color.g, color.b, color.a)
  for (let i = 0; i < entities.length; i++) {
	 translate(entities[i].p[0], entities[i].p[1])
	 let a = atan2(entities[i].d[1], entities[i].d[0]);
	 rotate(a);
	 translate(-entities[i].r, -entities[i].r)
	 image(entityImages[entities[i].i], 0, 0, entities[i].r*2, entities[i].r*2);
	 resetMatrix();
	}
}

function mouseMoved() {
  api.sendCommand("mousemove", {x: mouseX, y: mouseY});
}

function mouseClicked() {
	api.sendCommand("mouseClicked", {x: mouseX, y: mouseY});
}

function keyPressed() {
	api.sendCommand("keydown", {key: key, keyCode : keyCode});
}

function keyReleased() {
	api.sendCommand("keyup", {key: key, keyCode : keyCode});
}
