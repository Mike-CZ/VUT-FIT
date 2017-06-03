var runWindowManager = function () {
    var frameCount = 1;
    (function () {
        // najdi vsechny plochy
        desktopElems = document.getElementsByClassName("desktop");
        if (desktopElems.length > 0) {
            for (var x = 0 ; x < desktopElems.length; x++) {
                new MyDesktop(desktopElems[x]);
            }
        }

        function MyDesktop (element) {
            this.getWidth = function () { return element.offsetWidth; };
            this.getHeight = function () { return element.offsetHeight; };
            this.getXCoord = function () { return element.offsetLeft; };
            this.getYCoord = function () { return element.offsetTop; };
            this.getWindows = function () { return element.getElementsByClassName('window') };

            // najdi vsechny prislusna okna
            windowElems = element.getElementsByClassName("window");
            if (windowElems.length > 0) {
                for (var y = 0; y < windowElems.length; y++) {
                    new MyWindow(windowElems[y], this); // vytvor okna
                }
            }

        }

        function MyWindow(element, parent) {
            var resizeRadius = 10;
            var mouseDownFlag = 0;
            var holdX = 0;
            var holdY = 0;
            var oldHeight = 0;
            var oldWidth = 0;
            var maximalizedFlag = 0;
            var resizeDirection = 0;

            var title = (element.id.length > 0) ? element.id : "Okno " + (frameCount);
            if (element.id.length == 0)frameCount = frameCount + 1;

            var makeActive = function() {
                var windows = parent.getWindows();
                for (var i = 0; i < windows.length; i++) {
                    windows[i].style.zIndex = 1;
                    windows[i].style.opacity = 0.5;
                }
                element.style.zIndex = 99;
                element.style.opacity = 1;
            };

            var setCoords = function (x, y) {
                var pWidth = parent.getWidth();
                var pHeight = parent.getHeight();

                if (x < 0) {
                    x = 0;
                }
                else if ((x + element.offsetWidth) > pWidth) {
                    x = pWidth - element.offsetWidth - 1;
                }
                if (y < 0) {
                    y = 0;
                }
                else if ((y + element.offsetHeight) > pHeight) {
                    y = pHeight - element.offsetHeight - 1;
                }

                element.style.left = x + "px";
                element.style.top = y + "px";
            };

            var setHeight = function (amountY) {
                if (amountY <= 33) {
                    amountY = 33;
                }
                element.style.height =amountY + "px";
            };

            var setWidth = function (amountX) {
                if (amountX <= 50) {
                    amountX = 50;
                }
                element.style.width = amountX + "px";
            };

            var setX = function (x) {
                var pWidth = parent.getWidth();
                if (x < 0) {
                    x = 0;
                }
                else if ((x + element.offsetWidth) > pWidth) {
                    x = pWidth - element.offsetWidth - 1;
                }
                element.style.left = x + "px";
            };

            var setY = function (y) {
                var pHeight = parent.getHeight();
                if (y < 0) {
                    y = 0;
                }
                else if ((y + element.offsetHeight) > pHeight) {
                    y = pHeight - element.offsetHeight - 1;
                }
                element.style.top = y + "px";
            };

            var icons = '<table class="header-icons">' +
                '<tbody>' +
                '<tr><td><img class="max-btn" title="Cela obrazovka" src="images/fullscreen.png" /> </td><td><img title="Zavrit" class="exit-btn" src="images/cross.png" /></td></tr>' +
                '</tbody>' +
                '</table>';


            var header =    '<div class="window-title">' +
                title + icons +
                '</div>';

            // vlozi hlavicku do okna a obalime obsah wrapperem
            element.innerHTML = header + '<div class="window-content">' + element.innerHTML + '</div>';
            var head = element.getElementsByClassName("window-title")[0];

            // listener na tlacitko na smazani
            element.getElementsByClassName('exit-btn')[0].addEventListener('click', function () { element.parentNode.removeChild(element); });
            // listener na tlacitko na maximalizaci
            element.getElementsByClassName('max-btn')[0].addEventListener('click', function () {
                if (maximalizedFlag) {
                    maximalizedFlag = 0;
                    element.classList.remove("maximized");
                }
                else {
                    maximalizedFlag = 1;
                    element.classList.add("maximized");
                }
            });

            head.addEventListener('dblclick', function () {
                if (maximalizedFlag) {
                    maximalizedFlag = 0;
                    element.classList.remove("maximized");
                }
                else {
                    maximalizedFlag = 1;
                    element.classList.add("maximized");
                }
            });

            // event listener na hlavicku
            head.addEventListener("mousedown", function(e){
                e = e || window.event;
                e.preventDefault();

                // premistujeme pouze pokud nedrzime kraj okna
                if (!resizeDirection) {
                    var eXpos = parseInt(element.style.left) || element.offsetLeft;
                    var eYpos = parseInt(element.style.top) || element.offsetTop;

                    holdX = (e.clientX - parent.getXCoord()) - eXpos;
                    holdY = (e.clientY - parent.getYCoord()) - eYpos;
                    mouseDownFlag = 1;

                    // prida event listener na cely dokument, tim zajisti presouvani ikdyz neni kurzor na okne
                    document.addEventListener("mousemove", mouseMoveEvent, true);
                    document.addEventListener("mouseup", mouseUpListener, true);
                }
            }, true);

            // event listener na resize
            element.addEventListener("mousedown", function(e) {
                e = e || window.event;
                e.preventDefault();

                // udelej prvek aktivnim
                makeActive();

                holdX = parseInt(element.style.left) || element.offsetLeft;
                holdY = parseInt(element.style.top) || element.offsetTop;

                oldWidth = element.offsetWidth;
                oldHeight = element.offsetHeight;

                if (resizeDirection) {
                    mouseDownFlag = 1;

                    document.addEventListener("mousemove", mouseMoveResize, true);
                    document.addEventListener("mouseup", mouseUpResize, true);
                }

            }, true);

            element.addEventListener("mouseout", function(e) {
                e = e || window.event;
                e.preventDefault();
                if (!mouseDownFlag) {
                    document.getElementsByTagName("BODY")[0].style.cursor = 'default';
                    head.style.cursor = 'default';
                }
            });

            // detekce kraje
            element.addEventListener('mousemove', function(e){
                e = e || window.event;
                e.preventDefault();

                var src = e.target || e.srcElement;

                if (!mouseDownFlag) {
                    if ((! src.classList.contains("window-title")) && (! src.classList.contains("window")) || maximalizedFlag) {
                        document.getElementsByTagName("BODY")[0].style.cursor = 'default';
                        head.style.cursor = 'default';
                        return;
                    }
                    var xPos = e.offsetX || e.layerX;
                    var yPos = e.offsetY || e.layerY;

                    // servero-zapad
                    if ((yPos < resizeRadius) && (xPos < resizeRadius)) {
                        document.getElementsByTagName("BODY")[0].style.cursor = 'nw-resize';
                        head.style.cursor = 'nw-resize';
                        resizeDirection = 1;
                    }
                    // servero-vychod
                    else if ((yPos < resizeRadius) && (xPos > element.offsetWidth - resizeRadius)) {
                        document.getElementsByTagName("BODY")[0].style.cursor = 'ne-resize';
                        head.style.cursor = 'ne-resize';
                        resizeDirection = 2;
                    }
                    // jiho-zapad
                    else if ((yPos > element.offsetHeight - resizeRadius) && (xPos < resizeRadius)) {
                        document.getElementsByTagName("BODY")[0].style.cursor = 'sw-resize';
                        resizeDirection = 3;
                    }
                    // jiho-vychod
                    else if ((yPos > element.offsetHeight - resizeRadius) && (xPos > element.offsetWidth - resizeRadius)) {
                        document.getElementsByTagName("BODY")[0].style.cursor = 'se-resize';
                        resizeDirection = 4;
                    }
                    // sever
                    else if (yPos < resizeRadius) {
                        document.getElementsByTagName("BODY")[0].style.cursor = 'n-resize';
                        head.style.cursor = 'n-resize';
                        resizeDirection = 5;
                    }
                    // jih
                    else if (yPos > element.offsetHeight - resizeRadius) {
                        document.getElementsByTagName("BODY")[0].style.cursor = 's-resize';
                        resizeDirection = 6;
                    }
                    // zapad
                    else if (xPos < resizeRadius) {
                        document.getElementsByTagName("BODY")[0].style.cursor = 'w-resize';
                        head.style.cursor = 'w-resize';
                        resizeDirection = 7;
                    }
                    // vychod
                    else if (xPos > element.offsetWidth - resizeRadius) {
                        document.getElementsByTagName("BODY")[0].style.cursor = 'e-resize';
                        head.style.cursor = 'e-resize';
                        resizeDirection = 8;
                    }
                    else {
                        document.getElementsByTagName("BODY")[0].style.cursor = 'default';
                        head.style.cursor = 'default';
                        resizeDirection = 0;
                    }
                }
            }, true);

            // konec presouvani
            var mouseUpListener = function(e){
                e = e || window.event;
                e.preventDefault();
                mouseDownFlag = 0;
                document.removeEventListener("mousemove", mouseMoveEvent, true);
                document.removeEventListener("mouseup", mouseUpListener, true);
            };

            // konec zmeny velikosti okna
            var mouseUpResize = function(e) {
                e = e || window.event;
                e.preventDefault();
                mouseDownFlag = 0;
                document.getElementsByTagName("BODY")[0].style.cursor = 'default';
                document.removeEventListener("mousemove", mouseMoveResize, true);
                document.removeEventListener("mouseup", mouseUpResize, true);
            };

            // pohyb okna po plose
            var mouseMoveEvent = function(e){
                e = e || window.event;
                e.preventDefault();

                // hybeme oknem
                if (mouseDownFlag && ! maximalizedFlag) {
                    var cXpos = e.clientX - parent.getXCoord();
                    var cYpos = e.clientY - parent.getYCoord();

                    var newX = cXpos - holdX;
                    var newY = cYpos - holdY;

                    setCoords(newX, newY);
                }
            };

            // zmena velikosti
            var mouseMoveResize = function(e) {
                e = e || window.event;
                e.preventDefault();
                if (mouseDownFlag && ! maximalizedFlag) {

                    var currentX = e.clientX - parent.getXCoord();
                    var currentY = e.clientY - parent.getYCoord();

                    var amountY = parseInt(currentY - holdY);
                    var amountX = parseInt(currentX - holdX);

                    switch (resizeDirection) {
                        case 1:
                            if (currentY >= 0 && (holdY - currentY + oldHeight) >= 30) {
                                setY(currentY);
                                setHeight(holdY - currentY + oldHeight);
                            }
                            if (currentX >= 0 && (holdX - currentX + oldWidth) >= 55) {
                                setX(currentX);
                                setWidth(holdX - currentX + oldWidth);
                            }
                            break;
                        case 2:
                            if (currentY >= 0 && (holdY - currentY + oldHeight) >= 30) {
                                setY(currentY);
                                setHeight(holdY - currentY + oldHeight);
                            }
                            if (currentX < parent.getWidth()) {
                                setWidth(amountX);
                            }
                            break;
                        case 3:
                            if (currentY < parent.getHeight()) {
                                setHeight(amountY);
                            }
                            if (currentX >= 0 && (holdX - currentX + oldWidth) >= 55) {
                                setX(currentX);
                                setWidth(holdX - currentX + oldWidth);
                            }
                            break;
                        case 4 :
                            if (currentX < parent.getWidth()) {
                                setWidth(amountX);
                            }
                            if (currentY < parent.getHeight()) {
                                setHeight(amountY);
                            }
                            break;
                        case 5:
                            if (currentY >= 0 && (holdY - currentY + oldHeight) >= 30) {
                                setY(currentY);
                                setHeight(holdY - currentY + oldHeight);
                            }
                            break;
                        case 6:
                            if (currentY < parent.getHeight()) {
                                setHeight(amountY);
                            }
                            break;
                        case 7:
                            if (currentX >= 0 && (holdX - currentX + oldWidth) >= 55) {
                                setX(currentX);
                                setWidth(holdX - currentX + oldWidth);
                            }
                            break;
                        case 8:
                            if (currentX < parent.getWidth()) {
                                setWidth(amountX);
                            }
                            break;

                    }

                }
            };
            makeActive(); // aktkvni pri inicializaci bude vzdy posledni vlozene okno v dane plose
        }
    }());
};