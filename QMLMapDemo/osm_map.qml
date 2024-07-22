
import QtQuick.Window 2.2
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtLocation


Item {
    id: root
    // property string osm_path: ""

    Rectangle{


        color: "red"
        Text {
            id: name
            text: qsTr("text")
            color: "black"
        }
        Map {
            id: map
            anchors.fill: parent
            activeMapType: map.supportedMapTypes[1]
            zoomLevel: 1
            plugin: Plugin {
                name: "osm"
                PluginParameter { name: "osm.useragent"; value: "My great Qt OSM application" }
                PluginParameter { name: "osm.mapping.host"; value: "http://osm.tile.server.address/" }
                PluginParameter { name: "osm.mapping.copyright"; value: "All mine" }
                PluginParameter { name: "osm.routing.host"; value: "http://osrm.server.address/viaroute" }
                PluginParameter { name: "osm.geocoding.host"; value: "http://geocoding.server.address" }
                PluginParameter {
                    name: "osm.mapping.providersrepository.disabled"
                    value: "false"
                }
                PluginParameter {
                    name: "osm.mapping.providersrepository.address"
                    value: "http://maps-redirect.qt.io/osm/6.6/"
                }
            }
        }
    }
}
