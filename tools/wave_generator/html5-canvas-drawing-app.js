// Open.Theremin WAVE TABLE GENERATOR
// Copyright 2015 Urs Gaudenz
//
// Based on script example by William Malone (www.williammalone.com)
// https://github.com/williammalone/Simple-HTML5-Drawing-App
// and on http://www.gaudi.ch/WaveGenerator/ from Open Theremin project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/*jslint browser: true */
/*global G_vmlCanvasManager */

var drawingApp = (function () {
		
		"use strict";
		
		var canvas,
		context,
		canvasWidth = 400,
		canvasHeight = 300,
		colorPurple = "#cb3594",
		colorGreen = "#25eb11",
		colorYellow = "#ffcf33",
		colorBrown = "#986928",
		colorBlue = "#0020ff",
		outlineImage = new Image(),
		crayonImage = new Image(),
		markerImage = new Image(),
		eraserImage = new Image(),
		crayonBackgroundImage = new Image(),
		markerBackgroundImage = new Image(),
		eraserBackgroundImage = new Image(),
		crayonTextureImage = new Image(),
		clickX = [255],
		clickY = [],
		clickX_l,
		clickY_l,
		textfield,
		butgenerate,
		butscale,
		butcalculate,
		fieldparam1,
		fieldparam2,
		wave,
		clickColor = [],
		clickTool = [],
		clickSize = [],
		clickDrag = [],
		paint = false,
		curColor = colorGreen,
		curTool = "marker",
		curSize = "small",
		mediumStartX = 18,
		mediumStartY = 19,
		mediumImageWidth = 93,
		mediumImageHeight = 46,
		drawingAreaX = 111,
		drawingAreaY = 11,
		drawingAreaWidth = 255,
		drawingAreaHeight = 255,
		drawingBorder = 10,
		toolHotspotStartY = 23,
		toolHotspotHeight = 38,
		sizeHotspotStartY = 157,
		sizeHotspotHeight = 36,
		totalLoadResources = 8,
		curLoadResNum = 0,
		sizeHotspotWidthObject = {
			huge: 39,
			large: 25,
			normal: 18,
			small: 16
		},
		
		
	makemath=function()  {

var
 tableWidth=255,
 dacResolutionBits=8,
 brightness = fieldparam1.value, 		//you might put values from 0 to 255
 waveform = fieldparam2.value, 		//you might put values from 0 to 255 		// some preliminary parameter conditioning, don't touch!
 pi = 3.1415926535,
 br = (1.0 + 3.0 * brightness / 255.0) * 6.0 / pi,
 wf = 0.8 * waveform / 255.0,
 ampl = 512,
 dacMax = ampl - 1,
 dacMin = 0 - ampl,
 hp = pi * 2.0 / tableWidth,		// we need values for intermediate and final results in the function's equation
 y,
 t;    				// now the loop to generate the time steps and function values, and output the latter

for (t = -1; t < tableWidth; t++) {
clickX[t] = 512+ampl * Math.tanh((Math.asin(Math.sin(t * hp)) + wf) * br);
// formatted output, add a comma after each line but the last

}
	redraw();scale();

},




		
		play=function()  {
			
			var context;
			window.addEventListener('load', init, false);
			function init() {
				try {
					// Fix up for prefixing
					window.AudioContext = window.AudioContext||window.webkitAudioContext;
					context = new AudioContext();
				}
				catch(e) {
					alert('Web Audio API is not supported in this browser');
				}
			}
			
			
			var curveLength = 100;
			
			var curve1 = new Float32Array(curveLength);
			
			var curve2 = new Float32Array(curveLength);
			
			for (var i = 0; i < curveLength; i++)
				
				curve1[i] = Math.sin(Math.PI * i / curveLength);
			
			
			
			for (var i = 0; i < curveLength; i++)
				
				curve2[i] = Math.cos(Math.PI * i / curveLength);
			
			
			
			var waveTable = context.createWaveTable(curve1, curve2);
			
			source.setWaveTable(waveTable);
			
			source.connect(myAudioContext.destination);
			
			// play right now (0 seconds from now)
			
			source.noteOn(0);
			console.log("PLay Sound");
			
		},
		
		
		smooth=function()  {
			
			var i
			
			for (i = 0; i < 255; i += 1) {
				
				clickX[i]=clickX[i];
				
			}
			
		},   // END of Play Function
		
		scale=function()  {
			
			var i,maxn,minn,fac
			
			minn=2048;
			maxn=0;
			
			for (i = 0; i < 255; i += 1) {
				
				if (clickX[i]<minn) minn=clickX[i];
				if (clickX[i]>maxn) maxn=clickX[i];
				
			}
			
			console.log(minn);
			console.log(maxn);
			fac=(255/(maxn-minn));
			
			for (i = -1; i < 255; i += 1) {
				
				clickX[i]=(clickX[i]-minn)*fac;
				
				
			}
			

			redraw();

		},
		
		
		generate=function()  {
			
			var i
			wave="/* 255 entries full table, amplitude 1-255  Wavetable*/\n  \n const PROGMEM char Table[] = {\ \n";
			
			for (i = 0; i < 255-1; i += 1) {
				
				wave = wave.concat(Math.round((510-4*clickX[i])/2));
				wave = wave.concat(",\n");
				
			}
			
			wave = wave.concat(Math.round((510-4*clickX[254]))/2);
			wave = wave.concat("\n};\n");
			
			
			textfield.value=wave;
			
		},
		
		
		// Clears the canvas.
		clearCanvas = function () {
			
			context.clearRect(0, 0, canvasWidth, canvasHeight);
			
			//clickX[0]=10;
			//clickX[1]=254;
		},
		
		// Redraws the canvas.
		redraw = function () {
			
			var radius,
			i
			
			
			// Make sure required resources are loaded before redrawing
			if (curLoadResNum < totalLoadResources) {
				return;
			}
			
			clearCanvas();
			
			// Draw the marker tool background
			context.drawImage(markerBackgroundImage, 0, 0, canvasWidth, canvasHeight);
			
			// Keep the drawing in the drawing area
			context.save();
			context.beginPath();
			context.rect(drawingAreaX, drawingAreaY, drawingAreaWidth+2*drawingBorder, drawingAreaHeight);
			context.clip();
			
			// For each point drawn
			
			// Draw the outline image
			context.drawImage(outlineImage, drawingAreaX, drawingAreaY, drawingAreaWidth+2*drawingBorder, drawingAreaHeight);
			
			for (i = 0; i < 255; i += 1) {
				
				radius = 1;		// Set the drawing radius
				context.beginPath();  // Set the drawing path
				
				context.moveTo(drawingAreaX+i - 1+drawingBorder, drawingAreaY+clickX[i-1]); //draw path
				context.lineTo(drawingAreaX+i+drawingBorder, drawingAreaY+clickX[i]);
				
				context.strokeStyle = curColor; // Set color
				
				context.lineCap = "round";
				context.lineJoin = "round";
				context.lineWidth = radius;
				context.stroke();
			}
			
			for (i = 0; i < drawingBorder; i += 1) {
				
				radius = 1;		// Set the drawing radius
				context.beginPath();  // Set the drawing path
				
				context.moveTo(drawingAreaX+i - 1,drawingAreaY+ clickX[255-drawingBorder+i-1]); //draw path
				context.lineTo(drawingAreaX+i, drawingAreaY+clickX[255-drawingBorder+i]);
				
				context.strokeStyle = colorYellow; // Set color
				
				context.lineCap = "round";
				context.lineJoin = "round";
				context.lineWidth = radius;
				context.stroke();
			}
			
			for (i = 0; i < drawingBorder; i += 1) {
				
				radius = 1;		// Set the drawing radius
				context.beginPath();  // Set the drawing path
				
				context.moveTo(drawingAreaX+255+drawingBorder+i - 1,drawingAreaY+ clickX[i-1]); //draw path
				context.lineTo(drawingAreaX+255+drawingBorder+i, drawingAreaY+clickX[i]);
				
				context.strokeStyle = colorYellow; // Set color
				
				context.lineCap = "round";
				context.lineJoin = "round";
				context.lineWidth = radius;
				context.stroke();
			}
			
			
			context.closePath();
			
			
			//context.globalCompositeOperation = "source-over";// To erase instead of draw over with white
			context.restore();
			context.globalAlpha = 1; // No IE support
			
		},
		
		// Adds a point to the drawing array.
		// @param x
		// @param y
		// @param dragging
		addClick = function (x, y, dragging) {
			var i
			
			
			x=x-drawingAreaX-drawingBorder;
			y=y-drawingAreaY;
			clickX[x]=y;
			
			
			
			if (dragging) if ((clickX_l-x)>0)for (i = 0; i < (clickX_l-x); i += 1) {clickX[x+i]=y+i*(clickY_l-y)/ (clickX_l-x);}
			else for (i = 0; i < (x-clickX_l); i += 1) {clickX[x-i]=y+i*(clickY_l-y)/ (x-clickX_l);}
			
			clickX_l=x;
			clickY_l=y;
			
		},
		
		// Add mouse and touch event listeners to the canvas
		createUserEvents = function () {
			
			var press = function (e) {
				// Mouse down location
				var sizeHotspotStartX,
				mouseX = e.pageX - this.offsetLeft,
				mouseY = e.pageY - this.offsetTop;
				
				if (mouseX < drawingAreaX) { // Left of the drawing area
					if (mouseX > mediumStartX) {
						if (mouseY > mediumStartY && mouseY < mediumStartY + mediumImageHeight) {
							curColor = colorPurple;
						} else if (mouseY > mediumStartY + mediumImageHeight && mouseY < mediumStartY + mediumImageHeight * 2) {
							curColor = colorGreen;
						} else if (mouseY > mediumStartY + mediumImageHeight * 2 && mouseY < mediumStartY + mediumImageHeight * 3) {
							curColor = colorYellow;
						} else if (mouseY > mediumStartY + mediumImageHeight * 3 && mouseY < mediumStartY + mediumImageHeight * 4) {
							curColor = colorBrown;
						}
					}
				} else if (mouseX > drawingAreaX + drawingAreaWidth) { // Right of the drawing area
					
					if (mouseY > toolHotspotStartY) {
						if (mouseY > sizeHotspotStartY) {
							sizeHotspotStartX = drawingAreaX + drawingAreaWidth;
							if (mouseY < sizeHotspotStartY + sizeHotspotHeight && mouseX > sizeHotspotStartX) {
								if (mouseX < sizeHotspotStartX + sizeHotspotWidthObject.huge) {
									curSize = "huge";
								} else if (mouseX < sizeHotspotStartX + sizeHotspotWidthObject.large + sizeHotspotWidthObject.huge) {
									curSize = "large";
								} else if (mouseX < sizeHotspotStartX + sizeHotspotWidthObject.normal + sizeHotspotWidthObject.large + sizeHotspotWidthObject.huge) {
									curSize = "normal";
								} else if (mouseX < sizeHotspotStartX + sizeHotspotWidthObject.small + sizeHotspotWidthObject.normal + sizeHotspotWidthObject.large + sizeHotspotWidthObject.huge) {
									curSize = "small";
								}
							}
						} else {
							if (mouseY < toolHotspotStartY + toolHotspotHeight) {
								curTool = "crayon";
							} else if (mouseY < toolHotspotStartY + toolHotspotHeight * 2) {
								curTool = "marker";
							} else if (mouseY < toolHotspotStartY + toolHotspotHeight * 3) {
								curTool = "eraser";
							}
						}
					}
				}
				paint = true;
				addClick(mouseX, mouseY, false);
				redraw();
				
			},
			
			drag = function (e) {
				if (paint) {
					addClick(e.pageX - this.offsetLeft, e.pageY - this.offsetTop, true);
					redraw();
				}
				// Prevent the whole page from dragging if on mobile
				e.preventDefault();
			},
			
			release = function () {
				paint = false;
				redraw();
			},
			
			cancel = function () {
				paint = false;
			};
			
			
			
			
			
			// Add mouse event listeners to canvas element
			canvas.addEventListener("mousedown", press, false);
			canvas.addEventListener("mousemove", drag, false);
			canvas.addEventListener("mouseup", release);
			canvas.addEventListener("mouseout", cancel, false);
			
			// Add touch event listeners to canvas element
			canvas.addEventListener("touchstart", press, false);
			canvas.addEventListener("touchmove", drag, false);
			canvas.addEventListener("touchend", release, false);
			canvas.addEventListener("touchcancel", cancel, false);
			
			
		},
		
		// Calls the redraw function after all neccessary resources are loaded.
		resourceLoaded = function () {
			
			curLoadResNum += 1;
			if (curLoadResNum === totalLoadResources) {
				redraw();
				createUserEvents();
			}
		},
		
		// Creates a canvas element, loads images, adds events, and draws the canvas for the first time.
		init = function () {
			
			var i
			
			// Create the canvas (Neccessary for IE because it doesn't know what a canvas element is)
			canvas = document.createElement('canvas');
			canvas.setAttribute('width', canvasWidth);
			canvas.setAttribute('height', canvasHeight);
			canvas.setAttribute('id', 'canvas');
			document.getElementById('canvasDiv').appendChild(canvas);
			
			
			
			
			if (typeof G_vmlCanvasManager !== "undefined") {
				canvas = G_vmlCanvasManager.initElement(canvas);
			}
			context = canvas.getContext("2d"); // Grab the 2d canvas context
			// Note: The above code is a workaround for IE 8 and lower. Otherwise we could have used:
			//     context = document.getElementById('canvas').getContext("2d");
			
			// Load images
			crayonImage.onload = resourceLoaded;
			crayonImage.src = "images/crayon-outline.png";
			
			markerImage.onload = resourceLoaded;
			markerImage.src = "images/marker-outline.png";
			
			eraserImage.onload = resourceLoaded;
			eraserImage.src = "images/eraser-outline.png";
			
			crayonBackgroundImage.onload = resourceLoaded;
			crayonBackgroundImage.src = "images/crayon-background.png";
			
			markerBackgroundImage.onload = resourceLoaded;
			markerBackgroundImage.src = "images/marker-background.png";
			
			eraserBackgroundImage.onload = resourceLoaded;
			eraserBackgroundImage.src = "images/eraser-background.png";
			
			crayonTextureImage.onload = resourceLoaded;
			crayonTextureImage.src = "images/crayon-texture.png";
			
			outlineImage.onload = resourceLoaded;
			outlineImage.src = "images/watermelon-duck-outline.png";
			
			textfield=document.getElementById('TextDiv');
			butgenerate=document.getElementById('button_generate');
			butscale=document.getElementById('button_scale');
			butcalculate=document.getElementById('button_math');
			fieldparam1=document.getElementById('param1');
			fieldparam2=document.getElementById('param2');

			console.log("Hello world");
			console.log(fieldparam1.value);

			textfield.value="Press Generate to calculate values.";
			
 			for (i = 0; i < 255; i += 1) { clickX[i]=128;};
			redraw();
			butgenerate.addEventListener("click", function(){generate();});
			butscale.addEventListener("click", function(){scale();});
			butcalculate.addEventListener("click", function(){makemath();});
			
			
		};
		
		return {
			init: init
		};
}());
