

toast = function (text, type, autoHide = true, hasCloseButton = true) {
    console.log(`toast: ${text}`);
    Toastify({
        text,
        duration: autoHide ? 5000 : -1,
        newWindow: false,
        close: hasCloseButton,
        gravity: "top", // `top` or `bottom`
        position: "right", // `left`, `center` or `right`
        stopOnFocus: true, // Prevents dismissing of toast on hover                    
        className: `my-toastify-card ${type}`,
        onClick: function () { } // Callback after click
    }).showToast();
}

toast_error = function (from, exception, autoHide = false) {
    console.error(exception);
    toast(`ERROR (at ${from})\n${exception.message || exception.toString()}`, "danger", autoHide, true);
}