import QtQuick 2.12
import QtQuick.Controls 2.5
import com.jgaa.darkspeak 1.0

ApplicationWindow {
    id: mainWindow
    visible: true
    width: 640
    height: 480
    title: manager.programNameAndVersion

    header: MainToolBar {
        id: mainToolBar
        height: 32
        width: parent.width
    }

    SwipeView {
        id: swipeView
        anchors.fill: parent
        currentIndex: manager.currentPage

        onCurrentIndexChanged: {
            manager.currentPage = currentIndex
        }

        LogForm {
        }

        IdentitiesForm {
        }

        ContactsForm {
        }

        HomeForm {
        }

        ConversationsForm {
        }

        ChatForm {
        }

    }

    footer: TabBar {
        id: tabBar
        currentIndex: manager.currentPage
        visible: width > 400

        onCurrentIndexChanged: {
            manager.currentPage = currentIndex
        }

        TabButton {
            text: qsTr("Log")
        }
        TabButton {
            text: qsTr("Identities")
        }
        TabButton {
            text: qsTr("Contacts")
        }
        TabButton {
            text: qsTr("Home")
        }
        TabButton {
            text: qsTr("Conversations")
        }
        TabButton {
            text: qsTr("Chat")
        }
    }
}