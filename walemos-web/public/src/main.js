
// Firebase Setups
import { initializeApp } from "https://www.gstatic.com/firebasejs/10.1.0/firebase-app.js";
import { getDatabase, ref, onValue } from "https://www.gstatic.com/firebasejs/10.1.0/firebase-database.js";

const firebaseConfig = {
    apiKey: "AIzaSyBuzmdZ8ksbPnLysv3nCy5trnQ1rvJe3qQ",
    authDomain: "walemos-wpc.firebaseapp.com",
    databaseURL: "https://walemos-wpc-default-rtdb.firebaseio.com",
    projectId: "walemos-wpc",
    storageBucket: "walemos-wpc.appspot.com",
    messagingSenderId: "1037773381260",
    appId: "1:1037773381260:web:a386535cf6b26faef2e5a9",
    measurementId: "G-LVK135Q6P6"
};

const app = initializeApp(firebaseConfig);
const database = getDatabase(app);

function timeConverter(UNIX_timestamp){
  var a = new Date(UNIX_timestamp);
  var months = ['Jan','Feb','Mar','Apr','May','Jun','Jul','Aug','Sep','Oct','Nov','Dec'];
  var year = a.getFullYear();
  var month = months[a.getMonth()];
  var date = a.getDate();
  var hour = a.getHours();
  var min = a.getMinutes();
  var sec = a.getSeconds();
  var time = date + ' ' + month + ' ' + year + ' ' + hour + ':' + min + ':' + sec ;
  return time;
}

// Read from Realtime Database and update the values
let lastOn = 0;
let lastOff = 0;
const lastOnDisplay = document.querySelector("#box1");
const lastOffDisplay = document.querySelector("#box2");
const statusDisplay = document.querySelector("#box3");
function updateValues(){
    lastOnDisplay.innerHTML = "Last On:<br/>" + timeConverter(lastOn["Ts"]);
    lastOffDisplay.innerHTML = "Last Off:<br/>" + timeConverter(lastOff["Ts"]);
    statusDisplay.innerHTML = "Status:<br/>" + (lastOn["Ts"]>lastOff["Ts"] ?"Water!":"No Water");
    statusDisplay.style.background = (lastOn["Ts"]>lastOff["Ts"]?"red":"#56d3e3")
}
const lastOnRef = ref(database, 'data/lastOn');
const lastOffRef = ref(database, 'data/lastOff');
onValue(lastOnRef, (snapshot) => {
    lastOn = snapshot.val();
    updateValues();
});
onValue(lastOffRef, (snapshot) => {
    lastOff = snapshot.val();
    updateValues();
});