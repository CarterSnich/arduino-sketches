let esp8266DateTime;
let deviceDateTime;

let deviceTimeEl = document.querySelector('#device-clock > .time')
let deviceDateEl = document.querySelector('#device-clock > .date')

let esp8266TimeEl = document.querySelector('#esp8266-clock > .time')
let esp8266DateEl = document.querySelector('#esp8266-clock > .date')

let toastEl = document.getElementById('toast')
let syncBtn = document.getElementById('sync-btn')


if (window.location.hostname !== '127.0.0.1') {
    setInterval(() => {
        deviceDateTime = new Date();

        let hour = deviceDateTime.getHours();
        let AmPm = "AM";
        if (hour > 12) {
            hour = hour - 12;
            AmPm = "PM"
        }
        let mTime = `${(hour + "").padStart(2, '0')}:${(deviceDateTime.getMinutes()).padStart(2, '0')} ${AmPm}`
        let mDate = deviceDateTime.toLocaleDateString('en-US', {year: 'numeric', month: '2-digit', day: '2-digit'})

        deviceTimeEl.innerText = mTime
        deviceDateEl.innerText = mDate

    }, 100);

    setInterval(() => {
        fetch('/getdatetime', {
            method: 'GET',
            headers: {
                'Content-Type': 'application/json',
            }
        })
        .then(res => res.json())
        .then(data => {
            esp8266TimeEl.innerText = data.time;
            esp8266DateEl.innerText = data.date;
        })
        .catch(err => {
            toast('Failed to get datetime from ESP8266.')
        })
    }, 100);

}


syncBtn.addEventListener('click', function () {
    let body = new FormData();
    let now = new Date();
    let dateOptions = {month: 'short', day: 'numeric', year:'numeric'}
    let timeOptions = {hour: '2-digit', minute: '2-digit', second: '2-digit', }

    let dateValue = now.toLocaleDateString('en-US', dateOptions).replace(',', '')
    let timeValue = now.toLocaleDateString('en-US', timeOptions).replace("AM","").replace("PM","")

    body.append('date', dateValue)
    body.append('time', timeValue)

    fetch(`/updatedatetime`, {
        method: 'POST',
        body: body
    })
    .then(res => {
        if (!res.ok)  {
            console.error("Failed to update time.")
            toast("Failed to update time.", 5000)
        }
    })
    .then(data => {
        toast('ESP8266 time updated successfully.', 5000)
    })
    .catch(err => {
        toast("Failed to connect to web server.")
    })
})


function toast (message, timeout=0) {
    toastEl.innerText = message;
    toastEl.classList.add('show')

    if (timeout) {
        setTimeout(()=> {
            toastEl.classList.remove('show')
        }, timeout)
    }
}
