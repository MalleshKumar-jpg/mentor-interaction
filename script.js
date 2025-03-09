window.onload = function() {
    const url_params= new URLSearchParams(window.location.search);
    const role = url_params.get('role'); /* retrieving the type of user (mentor/mentee) from url*/
    
    if (document.body.id==='login-page'){
        document.getElementById('role').value = role;
        document.getElementById('login-form').addEventListener('submit', function (event){
        event.preventDefault();
        if (role){
            sessionStorage.setItem('role',role);
            event.preventDefault();
            if  (role==='mentor'){
                window.location.href = 'mentor_menteeslist.html';
            } else if (role==='mentee'){
                window.location.href = 'main.html';
            }
        }else{
            alert('No role detected. Please select your role')
        }
        });
    }

    if (document.body.id=="menteeslist-page"){
        const mentee_cards=document.querySelectorAll('.mentee-card');

        mentee_cards.forEach(card => {
            card.addEventListener('click',function(){
                const chosenMentee=card.getAttribute('data-mentee');
                sessionStorage.setItem('chosen_mentee',chosenMentee)
            });
        });
    }

    /* hiding buttons and elements based on the type of user */
    if (document.body.id === 'main-page'){
        const storedRole=sessionStorage.getItem('role');

        if (storedRole == 'mentor'){
            document.querySelectorAll('.mentee-only-btn').forEach(btn => btn.style.display='none'); /* hiding the class of mentee-only-btn */
            const sidebar_mentee_name=document.getElementById('mentee-name');
            sidebar_mentee_name.textContent=sessionStorage.getItem('chosen_mentee');
            
        }else if(storedRole == 'mentee'){
            document.querySelectorAll('.mentor-only-btn, .mentor-only-container').forEach(elem => elem.style.display='none'); /* hiding the class of mentor-only-btn , mentor-only container */
            
            const sidebar_mentee_name = document.getElementById('mentee-name'); /* hiding the sidebar mentee-name . we cant include this in above code , because mentee-name is an id*/
            sidebar_mentee_name.style.backgroundColor='darkgreen';
            sidebar_mentee_name.textContent='';

            const header_role=document.getElementById('user-role');
            header_role.textContent='MENTEE';
        }
        
    }
};