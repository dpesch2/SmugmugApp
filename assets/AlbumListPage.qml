import bb.cascades 1.2

Page {
    
    titleBar: TitleBar {
        id: titleBar
        title: qsTr ("SmugViewer")
    }

    Container {
        id: contAlbums

        ListView {
              
            function itemType(data, indexPath) {
                return "listItem";
            }
            
            onActivationChanged: {
            	 if ( active ) {
                    console.log( "show album " + indexPath[0] );
                    var selectedItem = albumListModel.data(indexPath);
                    mainPage.selectAlbum(selectedItem);
            	}
            }
            
            dataModel: ArrayDataModel {
            	id: albumListModel
            }
            
            listItemComponents: [
                                      
                 ListItemComponent {
                     type: "listItem"
     				
                     StandardListItem {
                         title: ListItemData.title 
                         description: ( ListItemData.subcategory == "" ? ListItemData.category : ListItemData.category + " / " +ListItemData.subcategory )
                         status: ListItemData.imageCount  
                         imageSource:  ListItemData.thumbImage != "" ? ListItemData.thumbImage : "asset:///images/album.jpg"
                     }
                }
             ]
         }
	} // Containder contGaleries
     
    // public         
    function fillAlbums(user, albums) {
        if (user == "") {
            titleBar.title = "SmugViewer"
        } else {
        	titleBar.title = "SmugViewer: " + user
        }
        albumListModel.clear();
        albumListModel.append(albums);
    }
} // Page