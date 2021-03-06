/**
 * @file MusicSourceModel.qml
 * @brief Music source menu list of elements.
 * Copyright (c) 2013, Ford Motor Company
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of the Ford Motor Company nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
import QtQuick 2.0

ListModel
{
    ListElement {
        title: "FM"
        qml: "./views/FMPlayerView.qml"
        appId: 0
    }
    ListElement {
        title: "AM"
        qml: "./views/AMPlayerView.qml"
        appId: 0
    }
    ListElement {
        title: "BT Audio"
        qml: "./views/BTPlayerView.qml"
        appId: 0
    }
    ListElement {
        title: "Sirius"
        qml: "./views/SiriusPlayerView.qml"
        appId: 0
    }
    ListElement {
        title: "CD"
        qml: "./views/CDPlayerView.qml"
        appId: 0
    }
    ListElement {
        title: "iPod"
        qml: "./views/IPodPlayerView.qml"
        appId: 0
    }
    ListElement {
        title: "USB"
        qml: "./views/UsbPlayerView.qml"
        appId: 0
    }
    ListElement {
        title: "LineIn"
        qml: "./views/LineInPlayerView.qml"
        appId: 0
    }
}
