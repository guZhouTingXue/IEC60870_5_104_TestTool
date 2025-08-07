import QtQuick
import QtQuick.Controls

MenuBar {

    Menu {
        title: "File"
        Action { text: "Load settings ..." }
        Action { text: "Save settings" }
        Action { text: "Save settings as ..." }
        MenuSeparator {}
        Action { text: "Exit" }

    }
    Menu {
        title: "Connection"
        Action { text: "Connect"; }
        Action { text: "Parameters ..." }

        MenuSeparator {}

        Action { text: "Automatic START DT"; checkable: true; checked: true }
        Action { text: "Send START DT" }
        Action { text: "Send STOP DT" }
        Action { text: "Automatic Reconnect" }

        MenuSeparator {}

        Action { text: "Download File ..." }
        Action { text: "Upload File ..." }
    }

    Menu {
        title: "Telegrams"
        Action { text: "Clear list"; }
        Action { text: "Pause recording" }
        Action { text: "Save telegrams ..."; }
        Action { text: "Load telegrams ..."; }

        MenuSeparator {}

        Action { text: "Show sent telegrams"; checkable: true; checked: true }
        Action { text: "Filter ..."; }
        Action { text: "Automatic Scrolling"; checkable: true; checked: true }

    }

    Menu {
        title: "Commands"
        Action { text: "System"; }
        Action { text: "Process"; }
        Action { text: "Parameters"; }
        Action { text: "Monitoring"; }
    }

    Menu {
        title: "Test"
        Action { text: "APCI commands"; }
        Action { text: "Flow Control"; }
    }

    Menu {
        title: "View"
        Action { text: "Object Viewer"; }
        Action { text: "Log"; }
        Action { text: "Raw Telegrams"; }
    }

    Menu {
        title: "Help"
        Action { text: "User Guide"; }
        Action { text: "About"; }
    }
}
