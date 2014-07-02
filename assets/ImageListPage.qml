import bb.cascades 1.2

Page {
    
    signal selectImage(variant selectedItem)

    titleBar: TitleBar {
        id: pageImageListTitle
    }

	Container {
 
	        ListView {
	              
	            function itemType(data, indexPath) { 
	                return "listItem";
	            }
	            
	            onActivationChanged: {
	                
	                if ( active ) {
	                    console.log( "show image " + indexPath[0] );
	                    var selectedItem = imageListModel.data(indexPath);
                        selectImage(selectedItem);
	                }
	            }
	
	            dataModel: ArrayDataModel {
	               id: imageListModel
	            }
	            
	            listItemComponents: [
	                 ListItemComponent {
	                     type: "listItem" 
	     				
	                     StandardListItem {
	                         title: ListItemData.filename  
	                         description: ListItemData.caption
	                         status: ListItemData.date
	                         imageSource:  ListItemData.thumbImage
	                     }
	                 }
	             ]
	        }
	        
	} // Container 
	
    // public   
    function fillImages(title, images) {
        pageImageListTitle.title = title;
        imageListModel.clear();
        imageListModel.append(images);
    } 

} // Page