import bb.cascades 1.2

Page {
    
    titleBar: TitleBar {
        title: qsTr ("Smugmug App")
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
    function fillAlbums(albums) {
        albumListModel.clear();
        albumListModel.append(albums);
    }
} // Page