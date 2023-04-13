// document.querySelector('.bg-r').style.opacity = '0';
// document.querySelector('.bg-g').style.opacity = '0';

document.querySelector('#frenos').addEventListener('click', async () => {
    const response = await fetch("/reversa", {
        method: "POST",
        headers: {
            "Content-Type": "application/json"
        },
        body: JSON.stringify({})
    });

    console.log(response);

})


document.querySelector('#luces').addEventListener('click', async () => {
    const response = await fetch("/luces", {
        method: "POST",
        headers: {
            "Content-Type": "application/json"
        },
        body: JSON.stringify({})
    });

    console.log(response);

})