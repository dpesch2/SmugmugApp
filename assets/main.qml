import bb.cascades 1.2
import bb.cascades.pickers 1.0
import bb.system 1.2 // for Toast
import Backend 1.0 // C++ backend

NavigationPane {
    id: mainPage

    property string selectedAlbumId: ""
    property string selectedImageId: ""
    
    Menu.definition: MenuDefinition {

        // Specify the actions that should be included in the menu
        actions: [
            ActionItem {
                title: qsTr("Preferences")
                //imageSource: "images/actionOneIcon.png"

                onTriggered: {
                    mainPage.push(pagePreferences);
                }
            }
           
        ] // end of actions list
    } // end of MenuDefinition

    // Main page
    AlbumListPage {
        id: pageAlbumList
 
        onCreationCompleted: {
            if ( settings.user == "" ) {
                mainPage.push(pagePreferences);
                toastOk.body = "Please set username."
                toastOk.show()
        	} else {
                comm.login(settings.user);
        	}
        }
    }

    attachedObjects: [
        
        // toast for messages
        SystemToast {
            id: toast
        },
        
        SystemToast {
            id: toastOk
            button.label: "OK"
        },
         
        Settings {
            id: settings
            
            onUserChanged: {
                    pageImageList.fillImages("",[]);
                    pageAlbumList.fillAlbums("", []);
                    comm.login(settings.user);
            }
        },

        Communicator {
            id: comm

            onAlbumsLoaded: {
                // Update album list
                var albums = comm.getAlbums();
                pageAlbumList.fillAlbums(settings.user, albums);
            }

            onImageLoaded: {

                // Update image list if updated album is selected
                if (mainPage.selectedAlbumId == albumId) {
                    // Update image on detailed view if it is selected
                    if (mainPage.selectedImageId == imageId && ! isThumb) {
                        pageImageDetail.imageSource = localPath;
                    }
                } 
            }

            onExifLoaded: {
                console.log("exif loaded");
                if (mainPage.selectedAlbumId == albumId && mainPage.selectedImageId == imageId) {
                    var image = comm.getImage(albumId, imageId)
                    pageImageDetail.showImageDetails(image);
                }
            }

            onImageSavedToShared: {
                toast.body = qsTr("Saving finished to ") + fullPath; 
                toast.show();
            }

            onError: {
                toastOk.body = errorMsg;
                toastOk.show();
            }
        },
        // end of Communicator

        // Edit preferences
        PreferrencesPage {
            id: pagePreferences
            
            defaultUser: settings.user

            titleBar: TitleBar {
                id: pagePreferencesTitle
                title: qsTr("Preferences")
                visibility: ChromeVisibility.Visible

                dismissAction: ActionItem {
                    title: qsTr("Cancel")
                    onTriggered: {
                        //addPage.addPageClose();
                        mainPage.pop();
                    }
                }

                acceptAction: ActionItem {
                    title: qsTr("Save")
                    onTriggered: {
                        settings.user = pagePreferences.defaultUser
                        settings.save();
                        mainPage.pop();
                    }
                }
            }

        }, // end of Page Preferences

        // List of all images for selected album
        ImageListPage {
            id: pageImageList
            
            attachedObjects: [
                
                FilePicker {
                    id: albumDownloadFilePicker
                    type : FileType.Picture
                    title : "Select Directory"
                    mode: FilePickerMode.SaverMultiple
                    directories : ["/accounts/1000/shared/misc"]
                    onFileSelected : {
                        console.log("FileSelected signal received : " + selectedFiles);
                        comm.downloadAlbum(mainPage.selectedAlbumId, selectedFiles[0] );
                    }
                }
            ]

            actions: [
                ActionItem {
                    title: qsTr("Save Album")
                    imageSource: "asset:///images/bb_action_saveas.png"
                    ActionBar.placement: ActionBarPlacement.OnBar
                    onTriggered: {
                        console.log("Save Album " + mainPage.selectedAlbumId);
                        var selectedAlbum = comm.getAlbum(mainPage.selectedAlbumId);
                        albumDownloadFilePicker.defaultSaveFileNames = selectedAlbum.title;
                        albumDownloadFilePicker.open()
                    }
                }
            ]

            paneProperties: NavigationPaneProperties {
                backButton: ActionItem {
                    title: qsTr("Back")
                    onTriggered: {
                        pageImageList = mainPage.pop();
                    }
                }
            }
        }, // end of Page pageImageList

        // Show large image and information about image
        ImageDetailsPage {
            id: pageImageDetail
 
            attachedObjects: [
                FilePicker {
                    id: imageDownloadFilePicker
                    type : FileType.Picture
                    title : "Select File"
                    mode: FilePickerMode.Saver
                    directories : ["/accounts/1000/shared/misc"]
                    onFileSelected : {
                        console.log("FileSelected signal received : " + selectedFiles);
                        var selectedImage = comm.getImage(mainPage.selectedAlbumId, mainPage.selectedImageId);
                        var selectedImageExt = getExtension(selectedImage.filename);
                        var selectedFileExt = getExtension(selectedFiles[0]);
                        var file = selectedFiles[0];
                        if ( selectedImageExt != selectedFileExt ) {
                            file = selectedFiles[0] + "." + selectedImageExt; 
                        }
                        comm.downloadImage(mainPage.selectedAlbumId, mainPage.selectedImageId, file);
                    }
                    
                    function getExtension(path) {
                        var lastDotIndex = path.lastIndexOf(".");
                        if (lastDotIndex == -1) {
                            return "";
                        }
                        var ext = path.substr(lastDotIndex + 1, path.length - lastDotIndex - 1);
                        return ext;
                    }
                }
            ]

            actions: [
                ActionItem {
                    title: qsTr("Save Image")
                    imageSource: "asset:///images/bb_action_saveas.png"
                    ActionBar.placement: ActionBarPlacement.OnBar
                    onTriggered: {
                        console.log("Save Image " + mainPage.selectedImageId);
                        var selectedImage = comm.getImage(mainPage.selectedAlbumId, mainPage.selectedImageId);
                        imageDownloadFilePicker.defaultSaveFileNames = selectedImage.filename; 
                        imageDownloadFilePicker.open();
                    }
                },
                ActionItem {
                    id: viewAndShareAction
                    title: qsTr("View & Share Image")
                    imageSource: "asset:///images/bb_action_share.png"
                    ActionBar.placement: ActionBarPlacement.OnBar
                    onTriggered: {
                        console.log("Share Image " + mainPage.selectedImageId);
                        comm.viewImage(mainPage.selectedAlbumId, mainPage.selectedImageId);
                    }
                }
            ]

            paneProperties: NavigationPaneProperties {
                backButton: ActionItem {
                    title: qsTr("Back")
                    onTriggered: {
                        pageImageDetail = mainPage.pop()
                    }
                }
            }
        } // end of Page pageImageDetail
    ]
    
    // load thumb images
    function loadThumbs(albumId, data) {
        for (var i = 0; i < data.length; i ++) {
            var item = data[i];
            
            // load only if the image is not in cache yet
            if (item.thumbImage == "") comm.loadImage(albumId, item.id, true /* isThumb */);
        }
    }

    // select album
    function selectAlbum(selectedItem) {
        var images = comm.getImages(selectedItem.id);
        
        // fill model with data
        pageImageList.fillImages(selectedItem.title, images);
        
        // ask for loading thumb images
        loadThumbs(selectedItem.id, images);
                
        mainPage.selectedAlbumId = selectedItem.id;
        mainPage.push(pageImageList);
    }
    
    // set detailed information to show about selected image
    function selectImage(selectedItem) {
        
        if (selectedItem.largeImage == "") {
            // image is not yet loaded, set thum and ask for full image and exif
            pageImageDetail.imageSource = selectedItem.thumbImage;
            comm.loadImage(mainPage.selectedAlbumId, selectedItem.id, false /* isThumb */);
            comm.loadExif(mainPage.selectedAlbumId, selectedItem.id);
        } else {
            // image is already loaded, show large
            pageImageDetail.imageSource = selectedItem.largeImage;
        }
        
        pageImageDetail.showImageDetails(selectedItem);
        mainPage.selectedImageId = selectedItem.id;
        mainPage.push(pageImageDetail);
    }
}
