/**
 * Developer: Stepan Burguchev
 * Date: 3/10/2016
 * Copyright: 2009-2016 Stepan Burguchev®
 *       All Rights Reserved
 * Published under the MIT license
 */

export default {
    durationToReadableString: function (duration) {
        let result = '';
        let days = duration.days();
        if (days) {
            result += `${days} days `;
        }
        let hours = duration.hours();
        if (hours) {
            result += `${hours} hours `;
        }
        let minutes = duration.minutes();
        if (minutes) {
            result += `${minutes} minutes `;
        }
        let seconds = duration.seconds();
        if (seconds) {
            result += `${seconds} seconds `;
        }
        return result;
    }
};
