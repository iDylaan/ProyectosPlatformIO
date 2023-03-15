(() => {

    document.querySelector('#led_r').addEventListener('change', (e) => {
        window.location.href = '/led_r?value=' + e.target.value;
    })
    
    document.querySelector('#led_g').addEventListener('change', (e) => {

    })
    
    document.querySelector('#led_b').addEventListener('change', (e) => {
        
    })

})();