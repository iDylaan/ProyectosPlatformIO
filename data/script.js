(() => {

    let led_r = document.querySelector("#led_r");
    let led_g = document.querySelector("#led_g");
    let led_b = document.querySelector("#led_b");
    const inputRanges = document.querySelectorAll("input[type='range']");
    
    setInterval(() => {
        getPotes();
    }, 2000);

    inputRanges.forEach(range => {
        range.addEventListener("click", async () => {
            document.querySelector('.bg-r').style.opacity = led_r.value / 255;
            document.querySelector('.bg-g').style.opacity = led_g.value / 255;
            document.querySelector('.bg-b').style.opacity = led_b.value / 255;

            leds = {
                led1: led_r.value,
                led2: led_g.value,
                led3: led_b.value
            }

            await fetch("/led", {
                method: "POST",
                headers: {
                    "Content-Type": "application/json"
                },
                body: JSON.stringify(leds)
            });
        });
    });


    async function getPotes() {
        const response = await fetch("/potenciometros");
        const data = await response.json();
        document.querySelector(".pote-1").textContent = data.pote1;
        document.querySelector(".pote-2").textContent = data.pote2;
        document.querySelector(".pote-3").textContent = data.pote3;
    }


    document.addEventListener("DOMContentLoaded", () => {
        led_r.value = 0
        led_g.value = 0
        led_b.value = 0
    });
})();