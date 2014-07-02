import bb.cascades 1.2

Page {
    property alias imageSource: imageView.imageSource

    titleBar: TitleBar {
        id: pageImageDetailTitle
    }
    
	Container {	     
	     layout: StackLayout {
	     }
	
	    ScrollView {
	        ImageView {
	            id: imageView
	            //imageSource: "asset:///images/loading.png"
	        }
	        scrollViewProperties {
	            scrollMode: ScrollMode.Both
	            pinchToZoomEnabled: true
	        }
	    }
	
	    ListView {
	        // Specify the data model from the attached objects list
	        dataModel: GroupDataModel {
	            id: groupDataModel
	            sortingKeys: [ "name" ]
	        }
	    }
	                     
	} // Container contImage
	
    function insertToListModel(prefix, text) {
        if (text != "") {
            groupDataModel.insert({
                    "name": prefix + text
            });
        } 
    } 

    function showImageDetails(selectedItem) {
        pageImageDetailTitle.title = selectedItem.filename;
        groupDataModel.clear();
        insertToListModel(qsTr("URL: "), selectedItem.largeURL);
        insertToListModel(qsTr("Caption: "), selectedItem.caption);
        insertToListModel(qsTr("Keywords: "), selectedItem.keywords);
        insertToListModel(qsTr("Date: "), selectedItem.dateProp);
        insertToListModel(qsTr("Aperture: "), selectedItem.aperture);
        insertToListModel(qsTr("DateTime: "), selectedItem.dateTime);
        insertToListModel(qsTr("ExposureTime: "), selectedItem.exposureTime);
        insertToListModel(qsTr("FocalLength: "), selectedItem.focalLength);
        insertToListModel(qsTr("ISO: "), selectedItem.iso);
    }
} // Page