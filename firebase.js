// ===============================
// Firebase Realtime Database Setup
// Executive Battery Dashboard
// ===============================

const firebaseConfig = {
  apiKey: "AIzaSyCNRNDstSXnbgSwGT0TKQdmJccEt4zhG8g",
  authDomain: "executive-battery-dashboard.firebaseapp.com",
  databaseURL: "https://executive-battery-dashboard-default-rtdb.asia-southeast1.firebasedatabase.app",
  projectId: "executive-battery-dashboard",
  storageBucket: "executive-battery-dashboard.firebasestorage.app",
  messagingSenderId: "762706049356",
  appId: "1:762706049356:web:bb5a154c224f89368d77e9",
  measurementId: "G-GXDRT567KY"
};

firebase.initializeApp(firebaseConfig);

const database = firebase.database();

const liveRef = database.ref("live");

liveRef.on("value", (snapshot) => {
  const data = snapshot.val();

  if (data) {
    updateDashboard(data);
  }
});
