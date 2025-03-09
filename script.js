window.onload = function() {
    const url_params= new URLSearchParams(window.location.search);
    const role = url_params.get('role');

    if (document.body.id==='login-page'){
        document.getElementById('role').value = role;
        document.getElementById('login-form').addEventListener('submit', function (event){
        event.preventDefault();
        if (role){
            sessionStorage.setItem('role',role);
            event.preventDefault();
            if  (role==='mentor'){
                window.location.href = 'mentor_menteeslist.html';
            } else if (role === 'mentee'){
                window.location.href = 'main.html';
            }
        }else{
            alert('No role detected. Please select your role')
        }
        });
    }
    
    if (document.body.id === 'main-page'){
        const storedRole=sessionStorage.getItem('role');

        if (storedRole == 'mentor'){
            document.querySelectorAll('.mentee-only-btn').forEach(btn => btn.style.display='none');
        }else if(storedRole == 'mentee'){
            document.querySelectorAll('.mentor-only-btn, .mentor-only-container').forEach(elem => elem.style.display='none');
            
            const sidebar_mentee_name = document.getElementById('mentee-name');
            sidebar_mentee_name.style.backgroundColor='darkgreen';
            sidebar_mentee_name.textContent='';
        }
    }
};